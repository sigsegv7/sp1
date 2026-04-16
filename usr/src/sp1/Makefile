#
# Copyright (c) 2026, Mirocom Laboratories
# All rights reserved.
#
# The following sources are CONFIDENTIAL and PROPRIETARY
# property of Mirocom Laboratories. Unauthorized copying,
# use, distrubution or modification of this file, in whole
# and in part, is strictly prohibited without the prior written
# consent from Mirocom Laboratories.
#

.PHONY: all
all: common $(ARCH)

.PHONY: common
common:
	cd common; $(MAKE)

.PHONY: $(ARCH)
$(ARCH):
	cd $(ARCH); $(MAKE)
