# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

GEN_SRCS += Firmware_Update_oma_object.c \
	Device_oma_object.c \
	LWM2M_Security_oma_object.c \
	LWM2M_Server_oma_object.c \
	Water_Flow_Readings_object.c \
	Water_Meter_High_Pressure_Alarm_object.c \
	Interval_Data_Delivery_object.c \
	Event_Data_Delivery_object.c \
	Delivery_Schedule_object.c \
	Test_Water_Meter_test.c
	
gen: $(GEN_SRCS)

clean:
	@rm -rf *_base.c *template_c *_base.h *template_h

define gt_oma_object_gen
@echo Generating $(subst _, ,$(notdir $*)) OMA Object
@gt-object-gen "$(subst _, ,$(notdir $*))" oma-objects-spec.json  ./
endef

%_oma_object.c:
	$(call gt_oma_object_gen)
	
define gt_object_gen
@echo Generating $(subst _, ,$(notdir $*)) Object
@gt-object-gen "$(subst _, ,$(notdir $*))" sew-oma-objects-spec.json  ./
endef

%_object.c:
	$(call gt_object_gen)

define test_object_gen
@echo Generating $(subst _, ,$(notdir $*)) Test Object
@gt-object-gen "$(subst _, ,$(notdir $*))" test-objects-spec.json  ./
endef

%_test.c:
	$(call test_object_gen)