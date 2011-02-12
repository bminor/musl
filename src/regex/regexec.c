/*
  regexec.c - TRE POSIX compatible matching functions (and more).

  Copyright (c) 2001-2006 Ville Laurikari <vl@iki.fi>.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <limits.h>

#include <regex.h>

#include "tre.h"

#include <assert.h>

static void
tre_fill_pmatch(size_t nmatch, regmatch_t pmatch[], int cflags,
		const tre_tnfa_t *tnfa, int *tags, int match_eo);

/***********************************************************************
 from tre-match-utils.h
***********************************************************************/

#define GET_NEXT_WCHAR() do {                                                 \
    prev_c = next_c; pos += pos_add_next;                                     \
    if ((pos_add_next = mbtowc(&next_c, str_byte, MB_LEN_MAX)) <= 0) {        \
        if (pos_add_next < 0) return REG_NOMATCH;                             \
        else pos_add_next++;                                                  \
    }                                                                         \
    str_byte += pos_add_next;                                                 \
  } while (0)

#define CHECK_ASSERTIONS(assertions)					      \
  (((assertions & ASSERT_AT_BOL)					      \
    && (pos > 0 || reg_notbol)						      \
    && (prev_c != L'\n' || !reg_newline))				      \
   || ((assertions & ASSERT_AT_EOL)					      \
       && (next_c != L'\0' || reg_noteol)				      \
       && (next_c != L'\n' || !reg_newline)))

/* Returns 1 if `t1' wins `t2', 0 otherwise. */
static int
tre_tag_order(int num_tags, tre_tag_direction_t *tag_directions,
	      int *t1, int *t2)
{
  int i;
  for (i = 0; i < num_tags; i++)
    {
      if (tag_directions[i] == TRE_TAG_MINIMIZE)
	{
	  if (t1[i] < t2[i])
	    return 1;
	  if (t1[i] > t2[i])
	    return 0;
	}
      else
	{
	  if (t1[i] > t2[i])
	    return 1;
	  if (t1[i] < t2[i])
	    return 0;
	}
    }
  /*  assert(0);*/
  return 0;
}

static int
tre_neg_char_classes_match(tre_ctype_t *classes, tre_cint_t wc, int icase)
{
  DPRINT(("neg_char_classes_test: %p, %d, %d\n", classes, wc, icase));
  while (*classes != (tre_ctype_t)0)
    if ((!icase && tre_isctype(wc, *classes))
	|| (icase && (tre_isctype(tre_toupper(wc), *classes)
		      || tre_isctype(tre_tolower(wc), *classes))))
      return 1; /* Match. */
    else
      classes++;
  return 0; /* No match. */
}


/***********************************************************************
 from tre-match-parallel.c
***********************************************************************/

/*
  This algorithm searches for matches basically by reading characters
  in the searched string one by one, starting at the beginning.	 All
  matching paths in the TNFA are traversed in parallel.	 When two or
  more paths reach the same state, exactly one is chosen according to
  tag ordering rules; if returning submatches is not required it does
  not matter which path is chosen.

  The worst case time required for finding the leftmost and longest
  match, or determining that there is no match, is always linearly
  dependent on the length of the text being searched.

  This algorithm cannot handle TNFAs with back referencing nodes.
  See `tre-match-backtrack.c'.
*/


typedef struct {
  tre_tnfa_transition_t *state;
  int *tags;
} tre_tnfa_reach_t;

typedef struct {
  int pos;
  int **tags;
} tre_reach_pos_t;


#ifdef TRE_DEBUG
static void
tre_print_reach(const tre_tnfa_t *tnfa, tre_tnfa_reach_t *reach, int num_tags)
{
  int i;

  while (reach->state != NULL)
    {
      DPRINT((" %p", (void *)reach->state));
      if (num_tags > 0)
	{
	  DPRINT(("/"));
	  for (i = 0; i < num_tags; i++)
	    {
	      DPRINT(("%d:%d", i, reach->tags[i]));
	      if (i < (num_tags-1))
		DPRINT((","));
	    }
	}
      reach++;
    }
  DPRINT(("\n"));

}
#endif /* TRE_DEBUG */

static reg_errcode_t
tre_tnfa_run_parallel(const tre_tnfa_t *tnfa, const void *string, int len,
		      int *match_tags, int eflags, int *match_end_ofs)
{
  /* State variables required by GET_NEXT_WCHAR. */
  tre_char_t prev_c = 0, next_c = 0;
  const char *str_byte = string;
  int pos = -1;
  int pos_add_next = 1;
#ifdef TRE_MBSTATE
  mbstate_t mbstate;
#endif /* TRE_MBSTATE */
  int reg_notbol = eflags & REG_NOTBOL;
  int reg_noteol = eflags & REG_NOTEOL;
  int reg_newline = tnfa->cflags & REG_NEWLINE;

  char *buf;
  tre_tnfa_transition_t *trans_i;
  tre_tnfa_reach_t *reach, *reach_next, *reach_i, *reach_next_i;
  tre_reach_pos_t *reach_pos;
  int *tag_i;
  int num_tags, i;

  int match_eo = -1;	   /* end offset of match (-1 if no match found yet) */
  int new_match = 0;
  int *tmp_tags = NULL;
  int *tmp_iptr;

#ifdef TRE_MBSTATE
  memset(&mbstate, '\0', sizeof(mbstate));
#endif /* TRE_MBSTATE */

  if (!match_tags)
    num_tags = 0;
  else
    num_tags = tnfa->num_tags;

  /* Allocate memory for temporary data required for matching.	This needs to
     be done for every matching operation to be thread safe.  This allocates
     everything in a single large block from the stack frame using alloca()
     or with malloc() if alloca is unavailable. */
  {
    int tbytes, rbytes, pbytes, xbytes, total_bytes;
    char *tmp_buf;
    /* Compute the length of the block we need. */
    tbytes = sizeof(*tmp_tags) * num_tags;
    rbytes = sizeof(*reach_next) * (tnfa->num_states + 1);
    pbytes = sizeof(*reach_pos) * tnfa->num_states;
    xbytes = sizeof(int) * num_tags;
    total_bytes =
      (sizeof(long) - 1) * 4 /* for alignment paddings */
      + (rbytes + xbytes * tnfa->num_states) * 2 + tbytes + pbytes;

    /* Allocate the memory. */
#ifdef TRE_USE_ALLOCA
    buf = alloca(total_bytes);
#else /* !TRE_USE_ALLOCA */
    buf = xmalloc(total_bytes);
#endif /* !TRE_USE_ALLOCA */
    if (buf == NULL)
      return REG_ESPACE;
    memset(buf, 0, total_bytes);

    /* Get the various pointers within tmp_buf (properly aligned). */
    tmp_tags = (void *)buf;
    tmp_buf = buf + tbytes;
    tmp_buf += ALIGN(tmp_buf, long);
    reach_next = (void *)tmp_buf;
    tmp_buf += rbytes;
    tmp_buf += ALIGN(tmp_buf, long);
    reach = (void *)tmp_buf;
    tmp_buf += rbytes;
    tmp_buf += ALIGN(tmp_buf, long);
    reach_pos = (void *)tmp_buf;
    tmp_buf += pbytes;
    tmp_buf += ALIGN(tmp_buf, long);
    for (i = 0; i < tnfa->num_states; i++)
      {
	reach[i].tags = (void *)tmp_buf;
	tmp_buf += xbytes;
	reach_next[i].tags = (void *)tmp_buf;
	tmp_buf += xbytes;
      }
  }

  for (i = 0; i < tnfa->num_states; i++)
    reach_pos[i].pos = -1;

  GET_NEXT_WCHAR();
  pos = 0;

  DPRINT(("length: %d\n", len));
  DPRINT(("pos:chr/code | states and tags\n"));
  DPRINT(("-------------+------------------------------------------------\n"));

  reach_next_i = reach_next;
  while (1)
    {
      /* If no match found yet, add the initial states to `reach_next'. */
      if (match_eo < 0)
	{
	  DPRINT((" init >"));
	  trans_i = tnfa->initial;
	  while (trans_i->state != NULL)
	    {
	      if (reach_pos[trans_i->state_id].pos < pos)
		{
		  if (trans_i->assertions
		      && CHECK_ASSERTIONS(trans_i->assertions))
		    {
		      DPRINT(("assertion failed\n"));
		      trans_i++;
		      continue;
		    }

		  DPRINT((" %p", (void *)trans_i->state));
		  reach_next_i->state = trans_i->state;
		  for (i = 0; i < num_tags; i++)
		    reach_next_i->tags[i] = -1;
		  tag_i = trans_i->tags;
		  if (tag_i)
		    while (*tag_i >= 0)
		      {
			if (*tag_i < num_tags)
			  reach_next_i->tags[*tag_i] = pos;
			tag_i++;
		      }
		  if (reach_next_i->state == tnfa->final)
		    {
		      DPRINT(("	 found empty match\n"));
		      match_eo = pos;
		      new_match = 1;
		      for (i = 0; i < num_tags; i++)
			match_tags[i] = reach_next_i->tags[i];
		    }
		  reach_pos[trans_i->state_id].pos = pos;
		  reach_pos[trans_i->state_id].tags = &reach_next_i->tags;
		  reach_next_i++;
		}
	      trans_i++;
	    }
	  DPRINT(("\n"));
	  reach_next_i->state = NULL;
	}
      else
	{
	  if (num_tags == 0 || reach_next_i == reach_next)
	    /* We have found a match. */
	    break;
	}

      /* Check for end of string. */
      if (!next_c) break;

      GET_NEXT_WCHAR();

#ifdef TRE_DEBUG
      DPRINT(("%3d:%2lc/%05d |", pos - 1, (tre_cint_t)prev_c, (int)prev_c));
      tre_print_reach(tnfa, reach_next, num_tags);
      DPRINT(("%3d:%2lc/%05d |", pos, (tre_cint_t)next_c, (int)next_c));
      tre_print_reach(tnfa, reach_next, num_tags);
#endif /* TRE_DEBUG */

      /* Swap `reach' and `reach_next'. */
      reach_i = reach;
      reach = reach_next;
      reach_next = reach_i;

      /* For each state in `reach' see if there is a transition leaving with
	 the current input symbol to a state not yet in `reach_next', and
	 add the destination states to `reach_next'. */
      reach_next_i = reach_next;
      for (reach_i = reach; reach_i->state; reach_i++)
	{
	  for (trans_i = reach_i->state; trans_i->state; trans_i++)
	    {
	      /* Does this transition match the input symbol? */
	      if (trans_i->code_min <= prev_c &&
		  trans_i->code_max >= prev_c)
		{
		  if (trans_i->assertions
		      && (CHECK_ASSERTIONS(trans_i->assertions)
			  /* Handle character class transitions. */
			  || ((trans_i->assertions & ASSERT_CHAR_CLASS)
			      && !(tnfa->cflags & REG_ICASE)
			      && !tre_isctype((tre_cint_t)prev_c,
					      trans_i->u.class))
			  || ((trans_i->assertions & ASSERT_CHAR_CLASS)
			      && (tnfa->cflags & REG_ICASE)
			      && (!tre_isctype(tre_tolower((tre_cint_t)prev_c),
					       trans_i->u.class)
				  && !tre_isctype(tre_toupper((tre_cint_t)prev_c),
						  trans_i->u.class)))
			  || ((trans_i->assertions & ASSERT_CHAR_CLASS_NEG)
			      && tre_neg_char_classes_match(trans_i->neg_classes,
							    (tre_cint_t)prev_c,
							    tnfa->cflags & REG_ICASE))))
		    {
		      DPRINT(("assertion failed\n"));
		      continue;
		    }

		  /* Compute the tags after this transition. */
		  for (i = 0; i < num_tags; i++)
		    tmp_tags[i] = reach_i->tags[i];
		  tag_i = trans_i->tags;
		  if (tag_i != NULL)
		    while (*tag_i >= 0)
		      {
			if (*tag_i < num_tags)
			  tmp_tags[*tag_i] = pos;
			tag_i++;
		      }

		  if (reach_pos[trans_i->state_id].pos < pos)
		    {
		      /* Found an unvisited node. */
		      reach_next_i->state = trans_i->state;
		      tmp_iptr = reach_next_i->tags;
		      reach_next_i->tags = tmp_tags;
		      tmp_tags = tmp_iptr;
		      reach_pos[trans_i->state_id].pos = pos;
		      reach_pos[trans_i->state_id].tags = &reach_next_i->tags;

		      if (reach_next_i->state == tnfa->final
			  && (match_eo == -1
			      || (num_tags > 0
				  && reach_next_i->tags[0] <= match_tags[0])))
			{
			  DPRINT(("  found match %p\n", trans_i->state));
			  match_eo = pos;
			  new_match = 1;
			  for (i = 0; i < num_tags; i++)
			    match_tags[i] = reach_next_i->tags[i];
			}
		      reach_next_i++;

		    }
		  else
		    {
		      assert(reach_pos[trans_i->state_id].pos == pos);
		      /* Another path has also reached this state.  We choose
			 the winner by examining the tag values for both
			 paths. */
		      if (tre_tag_order(num_tags, tnfa->tag_directions,
					tmp_tags,
					*reach_pos[trans_i->state_id].tags))
			{
			  /* The new path wins. */
			  tmp_iptr = *reach_pos[trans_i->state_id].tags;
			  *reach_pos[trans_i->state_id].tags = tmp_tags;
			  if (trans_i->state == tnfa->final)
			    {
			      DPRINT(("	 found better match\n"));
			      match_eo = pos;
			      new_match = 1;
			      for (i = 0; i < num_tags; i++)
				match_tags[i] = tmp_tags[i];
			    }
			  tmp_tags = tmp_iptr;
			}
		    }
		}
	    }
	}
      reach_next_i->state = NULL;
    }

  DPRINT(("match end offset = %d\n", match_eo));

#ifndef TRE_USE_ALLOCA
  if (buf)
    xfree(buf);
#endif /* !TRE_USE_ALLOCA */

  *match_end_ofs = match_eo;
  return match_eo >= 0 ? REG_OK : REG_NOMATCH;
}


/***********************************************************************
 from tre-match-backtrack.c
***********************************************************************/

/*
  This matcher is for regexps that use back referencing.  Regexp matching
  with back referencing is an NP-complete problem on the number of back
  references.  The easiest way to match them is to use a backtracking
  routine which basically goes through all possible paths in the TNFA
  and chooses the one which results in the best (leftmost and longest)
  match.  This can be spectacularly expensive and may run out of stack
  space, but there really is no better known generic algorithm.	 Quoting
  Henry Spencer from comp.compilers:
  <URL: http://compilers.iecc.com/comparch/article/93-03-102>

    POSIX.2 REs require longest match, which is really exciting to
    implement since the obsolete ("basic") variant also includes
    \<digit>.  I haven't found a better way of tackling this than doing
    a preliminary match using a DFA (or simulation) on a modified RE
    that just replicates subREs for \<digit>, and then doing a
    backtracking match to determine whether the subRE matches were
    right.  This can be rather slow, but I console myself with the
    thought that people who use \<digit> deserve very slow execution.
    (Pun unintentional but very appropriate.)

*/

typedef struct {
  int pos;
  const char *str_byte;
  tre_tnfa_transition_t *state;
  int state_id;
  int next_c;
  int *tags;
#ifdef TRE_MBSTATE
  mbstate_t mbstate;
#endif /* TRE_MBSTATE */
} tre_backtrack_item_t;

typedef struct tre_backtrack_struct {
  tre_backtrack_item_t item;
  struct tre_backtrack_struct *prev;
  struct tre_backtrack_struct *next;
} *tre_backtrack_t;

#ifdef TRE_MBSTATE
#define BT_STACK_MBSTATE_IN  stack->item.mbstate = (mbstate)
#define BT_STACK_MBSTATE_OUT (mbstate) = stack->item.mbstate
#else /* !TRE_MBSTATE */
#define BT_STACK_MBSTATE_IN
#define BT_STACK_MBSTATE_OUT
#endif /* !TRE_MBSTATE */


#ifdef TRE_USE_ALLOCA
#define tre_bt_mem_new		  tre_mem_newa
#define tre_bt_mem_alloc	  tre_mem_alloca
#define tre_bt_mem_destroy(obj)	  do { } while (0)
#else /* !TRE_USE_ALLOCA */
#define tre_bt_mem_new		  tre_mem_new
#define tre_bt_mem_alloc	  tre_mem_alloc
#define tre_bt_mem_destroy	  tre_mem_destroy
#endif /* !TRE_USE_ALLOCA */


#define BT_STACK_PUSH(_pos, _str_byte, _str_wide, _state, _state_id, _next_c, _tags, _mbstate) \
  do									      \
    {									      \
      int i;								      \
      if (!stack->next)							      \
	{								      \
	  tre_backtrack_t s;						      \
	  s = tre_bt_mem_alloc(mem, sizeof(*s));			      \
	  if (!s)							      \
	    {								      \
	      tre_bt_mem_destroy(mem);					      \
	      if (tags)							      \
		xfree(tags);						      \
	      if (pmatch)						      \
		xfree(pmatch);						      \
	      if (states_seen)						      \
		xfree(states_seen);					      \
	      return REG_ESPACE;					      \
	    }								      \
	  s->prev = stack;						      \
	  s->next = NULL;						      \
	  s->item.tags = tre_bt_mem_alloc(mem,				      \
					  sizeof(*tags) * tnfa->num_tags);    \
	  if (!s->item.tags)						      \
	    {								      \
	      tre_bt_mem_destroy(mem);					      \
	      if (tags)							      \
		xfree(tags);						      \
	      if (pmatch)						      \
		xfree(pmatch);						      \
	      if (states_seen)						      \
		xfree(states_seen);					      \
	      return REG_ESPACE;					      \
	    }								      \
	  stack->next = s;						      \
	  stack = s;							      \
	}								      \
      else								      \
	stack = stack->next;						      \
      stack->item.pos = (_pos);						      \
      stack->item.str_byte = (_str_byte);				      \
      stack->item.state = (_state);					      \
      stack->item.state_id = (_state_id);				      \
      stack->item.next_c = (_next_c);					      \
      for (i = 0; i < tnfa->num_tags; i++)				      \
	stack->item.tags[i] = (_tags)[i];				      \
      BT_STACK_MBSTATE_IN;						      \
    }									      \
  while (0)

#define BT_STACK_POP()							      \
  do									      \
    {									      \
      int i;								      \
      assert(stack->prev);						      \
      pos = stack->item.pos;						      \
      str_byte = stack->item.str_byte;					      \
      state = stack->item.state;					      \
      next_c = stack->item.next_c;					      \
      for (i = 0; i < tnfa->num_tags; i++)				      \
	tags[i] = stack->item.tags[i];					      \
      BT_STACK_MBSTATE_OUT;						      \
      stack = stack->prev;						      \
    }									      \
  while (0)

#undef MIN
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

static reg_errcode_t
tre_tnfa_run_backtrack(const tre_tnfa_t *tnfa, const void *string,
		       int len, int *match_tags,
		       int eflags, int *match_end_ofs)
{
  /* State variables required by GET_NEXT_WCHAR. */
  tre_char_t prev_c = 0, next_c = 0;
  const char *str_byte = string;
  int pos = 0;
  int pos_add_next = 1;
#ifdef TRE_MBSTATE
  mbstate_t mbstate;
#endif /* TRE_MBSTATE */
  int reg_notbol = eflags & REG_NOTBOL;
  int reg_noteol = eflags & REG_NOTEOL;
  int reg_newline = tnfa->cflags & REG_NEWLINE;

  /* These are used to remember the necessary values of the above
     variables to return to the position where the current search
     started from. */
  int next_c_start;
  const char *str_byte_start;
  int pos_start = -1;
#ifdef TRE_MBSTATE
  mbstate_t mbstate_start;
#endif /* TRE_MBSTATE */

  /* Compilation flags for this regexp. */
  int cflags = tnfa->cflags;

  /* End offset of best match so far, or -1 if no match found yet. */
  int match_eo = -1;
  /* Tag arrays. */
  int *next_tags, *tags = NULL;
  /* Current TNFA state. */
  tre_tnfa_transition_t *state;
  int *states_seen = NULL;

  /* Memory allocator to for allocating the backtracking stack. */
  tre_mem_t mem = tre_bt_mem_new();

  /* The backtracking stack. */
  tre_backtrack_t stack;

  tre_tnfa_transition_t *trans_i;
  regmatch_t *pmatch = NULL;
  int ret;

#ifdef TRE_MBSTATE
  memset(&mbstate, '\0', sizeof(mbstate));
#endif /* TRE_MBSTATE */

  if (!mem)
    return REG_ESPACE;
  stack = tre_bt_mem_alloc(mem, sizeof(*stack));
  if (!stack)
    {
      ret = REG_ESPACE;
      goto error_exit;
    }
  stack->prev = NULL;
  stack->next = NULL;

#ifdef TRE_USE_ALLOCA
  tags = alloca(sizeof(*tags) * tnfa->num_tags);
  pmatch = alloca(sizeof(*pmatch) * tnfa->num_submatches);
  states_seen = alloca(sizeof(*states_seen) * tnfa->num_states);
#else /* !TRE_USE_ALLOCA */
  tags = xmalloc(sizeof(*tags) * tnfa->num_tags);
  if (!tags)
    {
      ret = REG_ESPACE;
      goto error_exit;
    }
  pmatch = xmalloc(sizeof(*pmatch) * tnfa->num_submatches);
  if (!pmatch)
    {
      ret = REG_ESPACE;
      goto error_exit;
    }
  states_seen = xmalloc(sizeof(*states_seen) * tnfa->num_states);
  if (!states_seen)
    {
      ret = REG_ESPACE;
      goto error_exit;
    }
#endif /* !TRE_USE_ALLOCA */

 retry:
  {
    int i;
    for (i = 0; i < tnfa->num_tags; i++)
      {
	tags[i] = -1;
	if (match_tags)
	  match_tags[i] = -1;
      }
    for (i = 0; i < tnfa->num_states; i++)
      states_seen[i] = 0;
  }

  state = NULL;
  pos = pos_start;
  GET_NEXT_WCHAR();
  pos_start = pos;
  next_c_start = next_c;
  str_byte_start = str_byte;
#ifdef TRE_MBSTATE
  mbstate_start = mbstate;
#endif /* TRE_MBSTATE */

  /* Handle initial states. */
  next_tags = NULL;
  for (trans_i = tnfa->initial; trans_i->state; trans_i++)
    {
      DPRINT(("> init %p, prev_c %lc\n", trans_i->state, (tre_cint_t)prev_c));
      if (trans_i->assertions && CHECK_ASSERTIONS(trans_i->assertions))
	{
	  DPRINT(("assert failed\n"));
	  continue;
	}
      if (state == NULL)
	{
	  /* Start from this state. */
	  state = trans_i->state;
	  next_tags = trans_i->tags;
	}
      else
	{
	  /* Backtrack to this state. */
	  DPRINT(("saving state %d for backtracking\n", trans_i->state_id));
	  BT_STACK_PUSH(pos, str_byte, str_wide, trans_i->state,
			trans_i->state_id, next_c, tags, mbstate);
	  {
	    int *tmp = trans_i->tags;
	    if (tmp)
	      while (*tmp >= 0)
		stack->item.tags[*tmp++] = pos;
	  }
	}
    }

  if (next_tags)
    for (; *next_tags >= 0; next_tags++)
      tags[*next_tags] = pos;


  DPRINT(("entering match loop, pos %d, str_byte %p\n", pos, str_byte));
  DPRINT(("pos:chr/code | state and tags\n"));
  DPRINT(("-------------+------------------------------------------------\n"));

  if (state == NULL)
    goto backtrack;

  while (1)
    {
      tre_tnfa_transition_t *trans_i, *next_state;
      int empty_br_match;

      DPRINT(("start loop\n"));
      if (state == tnfa->final)
	{
	  DPRINT(("  match found, %d %d\n", match_eo, pos));
	  if (match_eo < pos
	      || (match_eo == pos
		  && match_tags
		  && tre_tag_order(tnfa->num_tags, tnfa->tag_directions,
				   tags, match_tags)))
	    {
	      int i;
	      /* This match wins the previous match. */
	      DPRINT(("	 win previous\n"));
	      match_eo = pos;
	      if (match_tags)
		for (i = 0; i < tnfa->num_tags; i++)
		  match_tags[i] = tags[i];
	    }
	  /* Our TNFAs never have transitions leaving from the final state,
	     so we jump right to backtracking. */
	  goto backtrack;
	}

#ifdef TRE_DEBUG
      DPRINT(("%3d:%2lc/%05d | %p ", pos, (tre_cint_t)next_c, (int)next_c,
	      state));
      {
	int i;
	for (i = 0; i < tnfa->num_tags; i++)
	  DPRINT(("%d%s", tags[i], i < tnfa->num_tags - 1 ? ", " : ""));
	DPRINT(("\n"));
      }
#endif /* TRE_DEBUG */

      /* Go to the next character in the input string. */
      empty_br_match = 0;
      trans_i = state;
      if (trans_i->state && trans_i->assertions & ASSERT_BACKREF)
	{
	  /* This is a back reference state.  All transitions leaving from
	     this state have the same back reference "assertion".  Instead
	     of reading the next character, we match the back reference. */
	  int so, eo, bt = trans_i->u.backref;
	  int bt_len;
	  int result;

	  DPRINT(("  should match back reference %d\n", bt));
	  /* Get the substring we need to match against.  Remember to
	     turn off REG_NOSUB temporarily. */
	  tre_fill_pmatch(bt + 1, pmatch, tnfa->cflags & !REG_NOSUB,
			  tnfa, tags, pos);
	  so = pmatch[bt].rm_so;
	  eo = pmatch[bt].rm_eo;
	  bt_len = eo - so;

	  if (len < 0)
	    {
	      result = strncmp((char*)string + so, str_byte - 1, bt_len);
	    }
	  else if (len - pos < bt_len)
	    result = 1;
	  else
	    result = memcmp((char*)string + so, str_byte - 1, bt_len);

	  /* We can ignore multibyte characters here because the backref
	     string is already aligned at character boundaries. */
	  if (result == 0)
	    {
	      /* Back reference matched.  Check for infinite loop. */
	      if (bt_len == 0)
		empty_br_match = 1;
	      if (empty_br_match && states_seen[trans_i->state_id])
		{
		  DPRINT(("  avoid loop\n"));
		  goto backtrack;
		}

	      states_seen[trans_i->state_id] = empty_br_match;

	      /* Advance in input string and resync `prev_c', `next_c'
		 and pos. */
	      DPRINT(("	 back reference matched\n"));
	      str_byte += bt_len - 1;
	      pos += bt_len - 1;
	      GET_NEXT_WCHAR();
	      DPRINT(("	 pos now %d\n", pos));
	    }
	  else
	    {
	      DPRINT(("	 back reference did not match\n"));
	      goto backtrack;
	    }
	}
      else
	{
	  /* Check for end of string. */
	  if (len < 0)
	    {
	      if (next_c == L'\0')
		goto backtrack;
	    }
	  else
	    {
	      if (pos >= len)
		goto backtrack;
	    }

	  /* Read the next character. */
	  GET_NEXT_WCHAR();
	}

      next_state = NULL;
      for (trans_i = state; trans_i->state; trans_i++)
	{
	  DPRINT(("  transition %d-%d (%c-%c) %d to %d\n",
		  trans_i->code_min, trans_i->code_max,
		  trans_i->code_min, trans_i->code_max,
		  trans_i->assertions, trans_i->state_id));
	  if (trans_i->code_min <= prev_c && trans_i->code_max >= prev_c)
	    {
	      if (trans_i->assertions
		  && (CHECK_ASSERTIONS(trans_i->assertions)
		      /* Handle character class transitions. */
		      || ((trans_i->assertions & ASSERT_CHAR_CLASS)
			  && !(cflags & REG_ICASE)
			  && !tre_isctype((tre_cint_t)prev_c, trans_i->u.class))
		      || ((trans_i->assertions & ASSERT_CHAR_CLASS)
			  && (cflags & REG_ICASE)
			  && (!tre_isctype(tre_tolower((tre_cint_t)prev_c),
					   trans_i->u.class)
			      && !tre_isctype(tre_toupper((tre_cint_t)prev_c),
					      trans_i->u.class)))
		      || ((trans_i->assertions & ASSERT_CHAR_CLASS_NEG)
			  && tre_neg_char_classes_match(trans_i->neg_classes,
							(tre_cint_t)prev_c,
							cflags & REG_ICASE))))
		{
		  DPRINT(("  assertion failed\n"));
		  continue;
		}

	      if (next_state == NULL)
		{
		  /* First matching transition. */
		  DPRINT(("  Next state is %d\n", trans_i->state_id));
		  next_state = trans_i->state;
		  next_tags = trans_i->tags;
		}
	      else
		{
		  /* Second mathing transition.	 We may need to backtrack here
		     to take this transition instead of the first one, so we
		     push this transition in the backtracking stack so we can
		     jump back here if needed. */
		  DPRINT(("  saving state %d for backtracking\n",
			  trans_i->state_id));
		  BT_STACK_PUSH(pos, str_byte, str_wide, trans_i->state,
				trans_i->state_id, next_c, tags, mbstate);
		  {
		    int *tmp;
		    for (tmp = trans_i->tags; tmp && *tmp >= 0; tmp++)
		      stack->item.tags[*tmp] = pos;
		  }
#if 0 /* XXX - it's important not to look at all transitions here to keep
	 the stack small! */
		  break;
#endif
		}
	    }
	}

      if (next_state != NULL)
	{
	  /* Matching transitions were found.  Take the first one. */
	  state = next_state;

	  /* Update the tag values. */
	  if (next_tags)
	    while (*next_tags >= 0)
	      tags[*next_tags++] = pos;
	}
      else
	{
	backtrack:
	  /* A matching transition was not found.  Try to backtrack. */
	  if (stack->prev)
	    {
	      DPRINT(("	 backtracking\n"));
	      if (stack->item.state->assertions && ASSERT_BACKREF)
		{
		  DPRINT(("  states_seen[%d] = 0\n",
			  stack->item.state_id));
		  states_seen[stack->item.state_id] = 0;
		}

	      BT_STACK_POP();
	    }
	  else if (match_eo < 0)
	    {
	      /* Try starting from a later position in the input string. */
	      /* Check for end of string. */
	      if (len < 0)
		{
		  if (next_c == L'\0')
		    {
		      DPRINT(("end of string.\n"));
		      break;
		    }
		}
	      else
		{
		  if (pos >= len)
		    {
		      DPRINT(("end of string.\n"));
		      break;
		    }
		}
	      DPRINT(("restarting from next start position\n"));
	      next_c = next_c_start;
#ifdef TRE_MBSTATE
	      mbstate = mbstate_start;
#endif /* TRE_MBSTATE */
	      str_byte = str_byte_start;
	      goto retry;
	    }
	  else
	    {
	      DPRINT(("finished\n"));
	      break;
	    }
	}
    }

  ret = match_eo >= 0 ? REG_OK : REG_NOMATCH;
  *match_end_ofs = match_eo;

 error_exit:
  tre_bt_mem_destroy(mem);
#ifndef TRE_USE_ALLOCA
  if (tags)
    xfree(tags);
  if (pmatch)
    xfree(pmatch);
  if (states_seen)
    xfree(states_seen);
#endif /* !TRE_USE_ALLOCA */

  return ret;
}


/***********************************************************************
 from regexec.c
***********************************************************************/

/* Fills the POSIX.2 regmatch_t array according to the TNFA tag and match
   endpoint values. */
static void
tre_fill_pmatch(size_t nmatch, regmatch_t pmatch[], int cflags,
		const tre_tnfa_t *tnfa, int *tags, int match_eo)
{
  tre_submatch_data_t *submatch_data;
  unsigned int i, j;
  int *parents;

  i = 0;
  if (match_eo >= 0 && !(cflags & REG_NOSUB))
    {
      /* Construct submatch offsets from the tags. */
      DPRINT(("end tag = t%d = %d\n", tnfa->end_tag, match_eo));
      submatch_data = tnfa->submatch_data;
      while (i < tnfa->num_submatches && i < nmatch)
	{
	  if (submatch_data[i].so_tag == tnfa->end_tag)
	    pmatch[i].rm_so = match_eo;
	  else
	    pmatch[i].rm_so = tags[submatch_data[i].so_tag];

	  if (submatch_data[i].eo_tag == tnfa->end_tag)
	    pmatch[i].rm_eo = match_eo;
	  else
	    pmatch[i].rm_eo = tags[submatch_data[i].eo_tag];

	  /* If either of the endpoints were not used, this submatch
	     was not part of the match. */
	  if (pmatch[i].rm_so == -1 || pmatch[i].rm_eo == -1)
	    pmatch[i].rm_so = pmatch[i].rm_eo = -1;

	  DPRINT(("pmatch[%d] = {t%d = %d, t%d = %d}\n", i,
		  submatch_data[i].so_tag, pmatch[i].rm_so,
		  submatch_data[i].eo_tag, pmatch[i].rm_eo));
	  i++;
	}
      /* Reset all submatches that are not within all of their parent
	 submatches. */
      i = 0;
      while (i < tnfa->num_submatches && i < nmatch)
	{
	  if (pmatch[i].rm_eo == -1)
	    assert(pmatch[i].rm_so == -1);
	  assert(pmatch[i].rm_so <= pmatch[i].rm_eo);

	  parents = submatch_data[i].parents;
	  if (parents != NULL)
	    for (j = 0; parents[j] >= 0; j++)
	      {
		DPRINT(("pmatch[%d] parent %d\n", i, parents[j]));
		if (pmatch[i].rm_so < pmatch[parents[j]].rm_so
		    || pmatch[i].rm_eo > pmatch[parents[j]].rm_eo)
		  pmatch[i].rm_so = pmatch[i].rm_eo = -1;
	      }
	  i++;
	}
    }

  while (i < nmatch)
    {
      pmatch[i].rm_so = -1;
      pmatch[i].rm_eo = -1;
      i++;
    }
}


/*
  Wrapper functions for POSIX compatible regexp matching.
*/

static int
tre_match(const tre_tnfa_t *tnfa, const void *string, size_t len,
	  size_t nmatch, regmatch_t pmatch[], int eflags)
{
  reg_errcode_t status;
  int *tags = NULL, eo;
  if (tnfa->num_tags > 0 && nmatch > 0)
    {
#ifdef TRE_USE_ALLOCA
      tags = alloca(sizeof(*tags) * tnfa->num_tags);
#else /* !TRE_USE_ALLOCA */
      tags = xmalloc(sizeof(*tags) * tnfa->num_tags);
#endif /* !TRE_USE_ALLOCA */
      if (tags == NULL)
	return REG_ESPACE;
    }

  /* Dispatch to the appropriate matcher. */
  if (tnfa->have_backrefs)
    {
      /* The regex has back references, use the backtracking matcher. */
      status = tre_tnfa_run_backtrack(tnfa, string, len, tags, eflags, &eo);
    }
  else
    {
      /* Exact matching, no back references, use the parallel matcher. */
      status = tre_tnfa_run_parallel(tnfa, string, len, tags, eflags, &eo);
    }

  if (status == REG_OK)
    /* A match was found, so fill the submatch registers. */
    tre_fill_pmatch(nmatch, pmatch, tnfa->cflags, tnfa, tags, eo);
#ifndef TRE_USE_ALLOCA
  if (tags)
    xfree(tags);
#endif /* !TRE_USE_ALLOCA */
  return status;
}

int
regexec(const regex_t *preg, const char *str,
	size_t nmatch, regmatch_t pmatch[], int eflags)
{
  return tre_match((void *)preg->TRE_REGEX_T_FIELD, str, -1,
                   nmatch, pmatch, eflags);
}

/* EOF */
