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
FINAL_IMAGE=${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=indicator/indicator.c pinGetLevel/pinGetLevel.c rx_trama/rx_trama.c serial/serial.c usart/usart.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/indicator/indicator.o ${OBJECTDIR}/pinGetLevel/pinGetLevel.o ${OBJECTDIR}/rx_trama/rx_trama.o ${OBJECTDIR}/serial/serial.o ${OBJECTDIR}/usart/usart.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/indicator/indicator.o.d ${OBJECTDIR}/pinGetLevel/pinGetLevel.o.d ${OBJECTDIR}/rx_trama/rx_trama.o.d ${OBJECTDIR}/serial/serial.o.d ${OBJECTDIR}/usart/usart.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/indicator/indicator.o ${OBJECTDIR}/pinGetLevel/pinGetLevel.o ${OBJECTDIR}/rx_trama/rx_trama.o ${OBJECTDIR}/serial/serial.o ${OBJECTDIR}/usart/usart.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=indicator/indicator.c pinGetLevel/pinGetLevel.c rx_trama/rx_trama.c serial/serial.c usart/usart.c main.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/atmega328p"



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

# The following macros may be used in the pre and post step lines
_/_=/
ShExtension=.sh
Device=ATmega328P
ProjectDir=/home/jcaf/Documents/PROYECTOS/ATMEL/C/Wilfredo/SistemaDiE-2025/firmware/firmware12junio2026/SistemaDieJunio2026.X
ProjectName=SistemaDieJunio2026
ConfName=default
ImagePath=dist/default/${IMAGE_TYPE}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
ImageDir=dist/default/${IMAGE_TYPE}
ImageName=SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif
MDFUHostPath=/home/jcaf/.mplab_ide/dev/v6.25/bin/linux/pymdfu-bin
PYFWImageBuilderPath=/home/jcaf/.mplab_ide/dev/v6.25/bin/linux/pyfwimagebuilder-bin

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [stdbuf -oL -eL avrdude -c usbasp -p m328p -v -V -U flash:w:"${ProjectDir}/dist/default/production/${ProjectName}.X.production.hex":i ]"
	@stdbuf -oL -eL avrdude -c usbasp -p m328p -v -V -U flash:w:"${ProjectDir}/dist/default/production/${ProjectName}.X.production.hex":i 
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=ATmega328P
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/indicator/indicator.o: indicator/indicator.c  .generated_files/flags/default/98aff27c5abe4195f9fe5b79dc973d8c7598bc23 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/indicator" 
	@${RM} ${OBJECTDIR}/indicator/indicator.o.d 
	@${RM} ${OBJECTDIR}/indicator/indicator.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/indicator/indicator.o.d" -MT "${OBJECTDIR}/indicator/indicator.o.d" -MT ${OBJECTDIR}/indicator/indicator.o  -o ${OBJECTDIR}/indicator/indicator.o indicator/indicator.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/pinGetLevel/pinGetLevel.o: pinGetLevel/pinGetLevel.c  .generated_files/flags/default/b054501766554872f32bc87bf8b03ff665980880 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/pinGetLevel" 
	@${RM} ${OBJECTDIR}/pinGetLevel/pinGetLevel.o.d 
	@${RM} ${OBJECTDIR}/pinGetLevel/pinGetLevel.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/pinGetLevel/pinGetLevel.o.d" -MT "${OBJECTDIR}/pinGetLevel/pinGetLevel.o.d" -MT ${OBJECTDIR}/pinGetLevel/pinGetLevel.o  -o ${OBJECTDIR}/pinGetLevel/pinGetLevel.o pinGetLevel/pinGetLevel.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/rx_trama/rx_trama.o: rx_trama/rx_trama.c  .generated_files/flags/default/a1a6cdaac12a33c146b80e72ec44bf99cbe627b4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/rx_trama" 
	@${RM} ${OBJECTDIR}/rx_trama/rx_trama.o.d 
	@${RM} ${OBJECTDIR}/rx_trama/rx_trama.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/rx_trama/rx_trama.o.d" -MT "${OBJECTDIR}/rx_trama/rx_trama.o.d" -MT ${OBJECTDIR}/rx_trama/rx_trama.o  -o ${OBJECTDIR}/rx_trama/rx_trama.o rx_trama/rx_trama.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/serial/serial.o: serial/serial.c  .generated_files/flags/default/723bf390c7a183fff00bd5e31c9e72bca2a5d1ce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/serial" 
	@${RM} ${OBJECTDIR}/serial/serial.o.d 
	@${RM} ${OBJECTDIR}/serial/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/serial/serial.o.d" -MT "${OBJECTDIR}/serial/serial.o.d" -MT ${OBJECTDIR}/serial/serial.o  -o ${OBJECTDIR}/serial/serial.o serial/serial.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/usart/usart.o: usart/usart.c  .generated_files/flags/default/c405bfd91b8ca14b79839eebd7295da229b2420a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usart" 
	@${RM} ${OBJECTDIR}/usart/usart.o.d 
	@${RM} ${OBJECTDIR}/usart/usart.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/usart/usart.o.d" -MT "${OBJECTDIR}/usart/usart.o.d" -MT ${OBJECTDIR}/usart/usart.o  -o ${OBJECTDIR}/usart/usart.o usart/usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/9d9d542c4a6f1b5c91bc306ae3460cce883590a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS} -g -DDEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o  -o ${OBJECTDIR}/main.o main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
else
${OBJECTDIR}/indicator/indicator.o: indicator/indicator.c  .generated_files/flags/default/8bbf083f7d4554aa7d4befdc0d17014cef577969 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/indicator" 
	@${RM} ${OBJECTDIR}/indicator/indicator.o.d 
	@${RM} ${OBJECTDIR}/indicator/indicator.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/indicator/indicator.o.d" -MT "${OBJECTDIR}/indicator/indicator.o.d" -MT ${OBJECTDIR}/indicator/indicator.o  -o ${OBJECTDIR}/indicator/indicator.o indicator/indicator.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/pinGetLevel/pinGetLevel.o: pinGetLevel/pinGetLevel.c  .generated_files/flags/default/1e7b42ef139c088bef62c7efda07a7e21d93e64d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/pinGetLevel" 
	@${RM} ${OBJECTDIR}/pinGetLevel/pinGetLevel.o.d 
	@${RM} ${OBJECTDIR}/pinGetLevel/pinGetLevel.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/pinGetLevel/pinGetLevel.o.d" -MT "${OBJECTDIR}/pinGetLevel/pinGetLevel.o.d" -MT ${OBJECTDIR}/pinGetLevel/pinGetLevel.o  -o ${OBJECTDIR}/pinGetLevel/pinGetLevel.o pinGetLevel/pinGetLevel.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/rx_trama/rx_trama.o: rx_trama/rx_trama.c  .generated_files/flags/default/ed8581dec6c630dd253a24d7c69e9fe735d6371 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/rx_trama" 
	@${RM} ${OBJECTDIR}/rx_trama/rx_trama.o.d 
	@${RM} ${OBJECTDIR}/rx_trama/rx_trama.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/rx_trama/rx_trama.o.d" -MT "${OBJECTDIR}/rx_trama/rx_trama.o.d" -MT ${OBJECTDIR}/rx_trama/rx_trama.o  -o ${OBJECTDIR}/rx_trama/rx_trama.o rx_trama/rx_trama.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/serial/serial.o: serial/serial.c  .generated_files/flags/default/63fd40d686ca7b6ec612d9803911db87286e1a92 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/serial" 
	@${RM} ${OBJECTDIR}/serial/serial.o.d 
	@${RM} ${OBJECTDIR}/serial/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/serial/serial.o.d" -MT "${OBJECTDIR}/serial/serial.o.d" -MT ${OBJECTDIR}/serial/serial.o  -o ${OBJECTDIR}/serial/serial.o serial/serial.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/usart/usart.o: usart/usart.c  .generated_files/flags/default/89cec3a9f39f927df36a4ba7ca4c584e653e929d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usart" 
	@${RM} ${OBJECTDIR}/usart/usart.o.d 
	@${RM} ${OBJECTDIR}/usart/usart.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/usart/usart.o.d" -MT "${OBJECTDIR}/usart/usart.o.d" -MT ${OBJECTDIR}/usart/usart.o  -o ${OBJECTDIR}/usart/usart.o usart/usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/b70fc30360cf2bfb7abac2858342a52faa6606 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega328p ${PACK_COMPILER_OPTIONS} ${PACK_COMMON_OPTIONS}  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o  -o ${OBJECTDIR}/main.o main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -DF_CPU=20000000UL
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega328p ${PACK_COMMON_OPTIONS}  -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  -DF_CPU=20000000UL $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.eep" || exit 0
	
	
	
	
else
${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega328p ${PACK_COMMON_OPTIONS}  -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  -DF_CPU=20000000UL $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.hex"
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/SistemaDieJunio2026.X.${IMAGE_TYPE}.eep" || exit 0
	
	
	
	
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
