// From
// https://github.com/nanobyte-dev/nanobyte_os/blob/master/src/kernel/util/binary.h

#pragma once

#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)
