/* pval.c -- Print Prefixed value.

Copyright 2019 Krypto-IT Jakub Juszczakiewicz

This file is part of the pval application.

pval is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pval is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with pval. If not, see http://www.gnu.org/licenses/.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char prefix_list[] = "kMGTPEZY";
unsigned int prefix_avail = 0;
unsigned long base = 1024;
unsigned int acomma = 2;
const char * eol = "";

int print_pval_auto(INTTYPE val)
{
  if (val < base)
  {
    unsigned long t = val;
    printf("%u%s", t, eol);
    return 0;
  }
  INTTYPE cur = val;

  for (int i = 0; prefix_list[i]; i++)
  {
    val = cur;
    cur /= base;

    if (cur < base)
    {
      char format[16] = { 0 };
      snprintf(format, 15, "%%1.%uf%%c%s%s", acomma, (base == 1024)?"i":"", eol);
      printf(format, (double)val / base, prefix_list[i]);
      return 0;
    }
  }

  char format[16] = { 0 };
  snprintf(format, 15, "%%1.%ufY%s%s", acomma, (base == 1024)?"i":"", eol);
  printf(format, (double)val / base);
  return 0;
}

int print_pval_low(INTTYPE val)
{
  if (val < base)
  {
    if (prefix_avail & 1)
    {
      unsigned long t = val;
      printf("%u%s", t, eol);
      return 0;
    }
    if (prefix_avail & 2)
    {
      char format[16] = { 0 };
      snprintf(format, 15, "%%1.%ufk%s%s", acomma, (base == 1024)?"i":"", eol);
      printf(format, (double)val / base);
      return 0;
    }
    for (int i = 2; prefix_list[i]; i++)
    {
      if (prefix_avail & (2 << i))
      {
        printf("0.00%s%c%s%s", (acomma == 2)?"":"0", prefix_list[i], (base == 1024)?"i":"", eol);
        return 0;
      }
    }
    printf("0%s", eol);
    return 0;
  }
  INTTYPE cur = val;
  INTTYPE val_last = 0;
  int i_last = -2;

  if (prefix_avail & 1)
  {
    i_last = -1;
    val_last = val;
  }

  for (int i = 0; prefix_list[i]; i++)
  {
    val = cur;
    cur /= base;

    if (prefix_avail & (2 << i))
    {
      val_last = val;
      i_last = i;
    }

    if (cur < base)
    {
      if (i_last == -1)
      {
        printf("%llu%s", (unsigned long long)val_last, eol);
        return 0;
      }
      if (i_last >= 0)
      {
        char format[16] = { 0 };
        snprintf(format, 15, "%%1.%uf%%c%s%s", acomma, (base == 1024)?"i":"", eol);
        printf(format, (double)val_last / base, prefix_list[i_last]);
        return 0;
      }
      if (prefix_list[i + 1])
      {
        if (prefix_avail & (4 << i))
        {
          char format[16] = { 0 };
          snprintf(format, 15, "%%1.%uf%%c%s%s", acomma, (base == 1024)?"i":"", eol);
          printf(format, (double)cur / base, prefix_list[i + 1]);
          return 0;
        }

        i += 2;
        for (; prefix_list[i]; i++)
        {
          if (prefix_avail & (2 << i))
          {
            printf("0.00%s%c%s%s", (acomma == 2)?"":"0", prefix_list[i], (base == 1024)?"i":"", eol);
            return 0;
          }
        }
        printf("0%s", eol);
        return 0;
      }
    }
  }

  char format[16] = { 0 };
  snprintf(format, 15, "%%1.%ufY%s%s", acomma, (base == 1024)?"i":"", eol);
  printf(format, (double)val / base);
  return 0;
}

int print_pval(const char * str)
{
  INTTYPE val = 0;
  INTTYPE last_val = 0;
  char minus = 0;

  const char * ptr = str;
  if (*ptr == '-')
  {
    minus = '-';
    ptr++;
  }

  while (*ptr)
  {
    last_val = val;
    if (*ptr < '0' || *ptr > '9')
    {
      fprintf(stderr, "%s is not valid decinal number\n", str);
      return 2;
    }
    val *= 10;
    val += (*ptr - '0');

    if (val < last_val)
    {
      fprintf(stderr, "Integer overflow - value %s to large\n", str);
      return 3;
    }

    ptr++;
  }

  if (minus)
  {
    putchar(minus);
  }
  if (!prefix_avail)
  {
    return print_pval_auto(val);
  }

  return print_pval_low(val);
}

void print_help(char * arg0)
{
  fprintf(stderr, "Usage: %s [-stnwxkMGTPEZY] decimal_number\n" \
                  "  -s  - SI prefixes (base = 1000 instead of 1024\n" \
                  "  -t  - 0.000 instead of 0.00 format\n" \
                  "  -n  - append '\\n' on end (instead of nothing)\n" \
                  "  -w  - append '\\r\\n' on end\n" \
                  "\n" \
                  " Allowed prefixess (default auto):\n" \
                  "  x   - without prefix (base ^ 0)\n" \
                  "  k   - kibi / kilo    (base ^ 1)\n" \
                  "  M   - mebi / mega    (base ^ 2)\n" \
                  "  G   - gibi / giga    (base ^ 3)\n" \
                  "  T   - tebi / tera    (base ^ 4)\n" \
                  "  P   - pebi / peta    (base ^ 5)\n" \
                  "  E   - exbi / exa     (base ^ 6)\n" \
                  "  Z   - zebi / zetta   (base ^ 7)\n" \
                  "  Y   - yobi / yotta   (base ^ 8)\n" \
                  "\n", arg0);
}

int main(int argc, char * argv[])
{
  for (int i = 1; i < argc; i++)
  {
    if (strlen(argv[i]) > 1 && argv[i][0] == '-' && 
         (argv[i][1] < '0' || argv[i][1] > '9'))
    {
      for (int j = 1; argv[i][j]; j++)
      {
        char arg = argv[i][j];
        if (arg == 'h')
        {
          print_help(argv[0]);
          return 0;
        }
        if (arg == 's')
        {
          base = 1000;
          continue;
        }
        if (arg == 't')
        {
          acomma = 3;
          continue;
        }
        if (arg == 'n')
        {
          eol = "\n";
          continue;
        }
        if (arg == 'w')
        {
          eol = "\r\n";
          continue;
        }
        if (arg == 'x')
        {
          prefix_avail |= 1;
          continue;
        }
        const char * pref = strchr(prefix_list, arg);
        if (!pref)
        {
          fprintf(stderr, "Invalid option: %c\n", arg);
          return 1;
        }
        prefix_avail |= 2 << (pref - prefix_list);
      }
      continue;
    }
    return print_pval(argv[i]);
  }

  return 0;
}
