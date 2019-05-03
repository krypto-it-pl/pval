# pval
Print preffiex value

Usage: pval [-stnwxkMGTPEZY] decimal_number
  -s  - SI prefixes (base = 1000 instead of 1024
  -t  - 0.000 instead of 0.00 format
  -n  - append '\n' on end (instead of nothing)
  -w  - append '\r\n' on end

 Allowed prefixess (default auto):
  x   - without prefix (base ^ 0)
  k   - kibi / kilo    (base ^ 1)
  M   - mebi / mega    (base ^ 2)
  G   - gibi / giga    (base ^ 3)
  T   - tebi / tera    (base ^ 4)
  P   - pebi / peta    (base ^ 5)
  E   - exbi / exa     (base ^ 6)
  Z   - zebi / zetta   (base ^ 7)
  Y   - yobi / yotta   (base ^ 8)
