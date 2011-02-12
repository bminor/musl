.data

.global timezone
.global __timezone
.global daylight
.global __daylight
.global tzname
.global __tzname

__timezone:
timezone:
	.long 0
.size timezone,.-timezone
.size __timezone,.-__timezone

__daylight:
daylight:
	.long 0
.size daylight,.-daylight
.size __daylight,.-__daylight

__tzname:
tzname:
	.long 0
	.long 0
.size tzname,.-tzname
.size __tzname,.-__tzname
