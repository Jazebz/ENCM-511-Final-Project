#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.c FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S FreeRTOS/portable/MemMang/heap_1.c FreeRTOS/croutine.c FreeRTOS/event_groups.c FreeRTOS/list.c FreeRTOS/queue.c FreeRTOS/stream_buffer.c FreeRTOS/tasks.c FreeRTOS/timers.c main.c uart.c FreeRTOS/ADC.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o ${OBJECTDIR}/FreeRTOS/croutine.o ${OBJECTDIR}/FreeRTOS/event_groups.o ${OBJECTDIR}/FreeRTOS/list.o ${OBJECTDIR}/FreeRTOS/queue.o ${OBJECTDIR}/FreeRTOS/stream_buffer.o ${OBJECTDIR}/FreeRTOS/tasks.o ${OBJECTDIR}/FreeRTOS/timers.o ${OBJECTDIR}/main.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/FreeRTOS/ADC.o
POSSIBLE_DEPFILES=${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o.d ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o.d ${OBJECTDIR}/FreeRTOS/croutine.o.d ${OBJECTDIR}/FreeRTOS/event_groups.o.d ${OBJECTDIR}/FreeRTOS/list.o.d ${OBJECTDIR}/FreeRTOS/queue.o.d ${OBJECTDIR}/FreeRTOS/stream_buffer.o.d ${OBJECTDIR}/FreeRTOS/tasks.o.d ${OBJECTDIR}/FreeRTOS/timers.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/uart.o.d ${OBJECTDIR}/FreeRTOS/ADC.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o ${OBJECTDIR}/FreeRTOS/croutine.o ${OBJECTDIR}/FreeRTOS/event_groups.o ${OBJECTDIR}/FreeRTOS/list.o ${OBJECTDIR}/FreeRTOS/queue.o ${OBJECTDIR}/FreeRTOS/stream_buffer.o ${OBJECTDIR}/FreeRTOS/tasks.o ${OBJECTDIR}/FreeRTOS/timers.o ${OBJECTDIR}/main.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/FreeRTOS/ADC.o

# Source Files
SOURCEFILES=FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.c FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S FreeRTOS/portable/MemMang/heap_1.c FreeRTOS/croutine.c FreeRTOS/event_groups.c FreeRTOS/list.c FreeRTOS/queue.c FreeRTOS/stream_buffer.c FreeRTOS/tasks.c FreeRTOS/timers.c main.c uart.c FreeRTOS/ADC.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GA702
MP_LINKER_FILE_OPTION=,--script=p24FJ256GA702.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o: FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.c  .generated_files/flags/default/15519addbee98e38fa467439a536e1717d7b04e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o: FreeRTOS/portable/MemMang/heap_1.c  .generated_files/flags/default/3218a10234baca55e1548bb8b36486fff013f4c4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MemMang" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/croutine.o: FreeRTOS/croutine.c  .generated_files/flags/default/77b0241e8b1f7dbd3effe323c65af099f555f35d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/croutine.c  -o ${OBJECTDIR}/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/croutine.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/event_groups.o: FreeRTOS/event_groups.c  .generated_files/flags/default/e36d1e106e3a326f34f924d64a155d3ae4959a2d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/event_groups.c  -o ${OBJECTDIR}/FreeRTOS/event_groups.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/event_groups.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/list.o: FreeRTOS/list.c  .generated_files/flags/default/59e58e06cecba2d009ae4275fce89e3265ed93d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/list.c  -o ${OBJECTDIR}/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/list.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/queue.o: FreeRTOS/queue.c  .generated_files/flags/default/16753168dc1a87ac0cce30c3b56a4993928a5bd6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/queue.c  -o ${OBJECTDIR}/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/queue.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/stream_buffer.o: FreeRTOS/stream_buffer.c  .generated_files/flags/default/833088ad443487714ea3d3ff92e79ca2a94997e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/stream_buffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/stream_buffer.c  -o ${OBJECTDIR}/FreeRTOS/stream_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/stream_buffer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/tasks.o: FreeRTOS/tasks.c  .generated_files/flags/default/ab690e0bd9ada07ae63ffd5ff7731c71191a73f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/tasks.c  -o ${OBJECTDIR}/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/tasks.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/timers.o: FreeRTOS/timers.c  .generated_files/flags/default/eb607e951e0c0ad6d49769797d4bafba971681e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/timers.c  -o ${OBJECTDIR}/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/7cdd6a1315282a3e3e714172608c260af8985db0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/uart.o: uart.c  .generated_files/flags/default/c799d0c4c7a68212e5477caaffee0654c98c8d5b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart.c  -o ${OBJECTDIR}/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/ADC.o: FreeRTOS/ADC.c  .generated_files/flags/default/7df80260fff10a8239141e694aee22bc33066ba1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/ADC.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/ADC.c  -o ${OBJECTDIR}/FreeRTOS/ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/ADC.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o: FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.c  .generated_files/flags/default/57467f7c2744ddac6847da5b86bfee9eec53a657 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/port.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o: FreeRTOS/portable/MemMang/heap_1.c  .generated_files/flags/default/2cb3c7ca6573124524ca4192feaefac069baad75 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MemMang" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/croutine.o: FreeRTOS/croutine.c  .generated_files/flags/default/5a36c9240f2878de1d95cf09ee15d5d938158c64 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/croutine.c  -o ${OBJECTDIR}/FreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/croutine.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/event_groups.o: FreeRTOS/event_groups.c  .generated_files/flags/default/10027d1388b915d6f1d9969b29dd7f3206f82b5b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/event_groups.c  -o ${OBJECTDIR}/FreeRTOS/event_groups.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/event_groups.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/list.o: FreeRTOS/list.c  .generated_files/flags/default/362ed15f01aa760c60afb9a5d04107470e53b248 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/list.c  -o ${OBJECTDIR}/FreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/list.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/queue.o: FreeRTOS/queue.c  .generated_files/flags/default/bc6745c566a87c8d357e254999a3980cac60c0d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/queue.c  -o ${OBJECTDIR}/FreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/queue.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/stream_buffer.o: FreeRTOS/stream_buffer.c  .generated_files/flags/default/cc1869bb8087dd7ba278fd5615105ccb3660b9f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/stream_buffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/stream_buffer.c  -o ${OBJECTDIR}/FreeRTOS/stream_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/stream_buffer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/tasks.o: FreeRTOS/tasks.c  .generated_files/flags/default/f025b7645e6025035e8d0b018531c55baa5708e3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/tasks.c  -o ${OBJECTDIR}/FreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/tasks.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/timers.o: FreeRTOS/timers.c  .generated_files/flags/default/12380b8508e306a066c627b30fbc99aead28c10e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/timers.c  -o ${OBJECTDIR}/FreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/b3d3451f65852b9c300019f2075b8b17775b4059 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/uart.o: uart.c  .generated_files/flags/default/64ad1c632aac0d6beede51be397bb1ee1eda9d7f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart.c  -o ${OBJECTDIR}/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/FreeRTOS/ADC.o: FreeRTOS/ADC.c  .generated_files/flags/default/2224dc47e4dfc0674704b6231e91a51f9b88c43 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/ADC.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FreeRTOS/ADC.c  -o ${OBJECTDIR}/FreeRTOS/ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/ADC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o: FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  .generated_files/flags/default/25560e9f0f9eea38ead953d5eae6712ea00195 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d"  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -Wa,-MD,"${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o: FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  .generated_files/flags/default/e0d0b87afd48c5a4fd096ebf093d92933d79675c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)    -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -Wa,-MD,"${OBJECTDIR}/FreeRTOS/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC"  -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"./FreeRTOS/include" -I"./" -I"./FreeRTOS/portable/MPLAB/PIC24_dsPIC" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/ENCM-511-Final-Project-.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
