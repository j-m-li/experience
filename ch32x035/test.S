# 2025 PUBLIC DOMAIN by JML

# BLINK LED on PA0

	.section .init,"ax",@progbits
	.global	_start
	.align 1
_start:
	j handle_reset

	.section .vector,"ax",@progbits
	.align 1
_vector_base:
	.option norvc;
	.word _start
	.word 0
	.word NMI_Handler
	.word HardFault_Handler
	.word 0
	.word Ecall_M_Mode_Handler
	.word 0
	.word 0
	.word Ecall_U_Mode_Handler
	.word Break_Point_Handler
	.word 0
	.word 0
	.word SysTick_Handler
	.word 0
	.word SW_Handler
	.word 0
	 # External Interrupts
	.word WWDG_IRQHandler
	.word PVD_IRQHandler
	.word FLASH_IRQHandler
	.word 0
	.word EXTI7_0_IRQHandler
	.word AWU_IRQHandler
	.word DMA1_Channel1_IRQHandler
	.word DMA1_Channel2_IRQHandler
	.word DMA1_Channel3_IRQHandler
	.word DMA1_Channel4_IRQHandler
	.word DMA1_Channel5_IRQHandler
	.word DMA1_Channel6_IRQHandler
	.word DMA1_Channel7_IRQHandler
	.word ADC1_IRQHandler
	.word I2C1_EV_IRQHandler
	.word I2C1_ER_IRQHandler
	.word USART1_IRQHandler
	.word SPI1_IRQHandler
	.word TIM1_BRK_IRQHandler
	.word TIM1_UP_IRQHandler
	.word TIM1_TRG_COM_IRQHandler
	.word TIM1_CC_IRQHandler
	.word TIM2_UP_IRQHandler
	.word USART2_IRQHandler
	.word EXTI15_8_IRQHandler
	.word EXTI25_16_IRQHandler
	.word USART3_IRQHandler
	.word USART4_IRQHandler
	.word DMA1_Channel8_IRQHandler
	.word USBFS_IRQHandler
	.word USBFSWakeUp_IRQHandler
	.word PIOC_IRQHandler
	.word OPA_IRQHandler
	.word USBPD_IRQHandler
	.word USBPDWakeUp_IRQHandler
	.word TIM2_CC_IRQHandler
	.word TIM2_TRG_COM_IRQHandler
	.word TIM2_BRK_IRQHandler
	.word TIM3_IRQHandler

	#.option rvc;
	.section .text.vector_handler, "ax", @progbits
	.weak NMI_Handler
	.weak HardFault_Handler
	.weak Ecall_M_Mode_Handler
	.weak Ecall_U_Mode_Handler
	.weak Break_Point_Handler
	.weak SysTick_Handler
	.weak SW_Handler
	.weak WWDG_IRQHandler
	.weak PVD_IRQHandler
	.weak FLASH_IRQHandler
	.weak EXTI7_0_IRQHandler
	.weak AWU_IRQHandler
	.weak DMA1_Channel1_IRQHandler
	.weak DMA1_Channel2_IRQHandler
	.weak DMA1_Channel3_IRQHandler
	.weak DMA1_Channel4_IRQHandler
	.weak DMA1_Channel5_IRQHandler
	.weak DMA1_Channel6_IRQHandler
	.weak DMA1_Channel7_IRQHandler
	.weak ADC1_IRQHandler
	.weak I2C1_EV_IRQHandler
	.weak I2C1_ER_IRQHandler
	.weak USART1_IRQHandler
	.weak SPI1_IRQHandler
	.weak TIM1_BRK_IRQHandler
	.weak TIM1_UP_IRQHandler
	.weak TIM1_TRG_COM_IRQHandler
	.weak TIM1_CC_IRQHandler
	.weak TIM2_UP_IRQHandler
	.weak USART2_IRQHandler
	.weak EXTI15_8_IRQHandler
	.weak EXTI25_16_IRQHandler
	.weak USART3_IRQHandler
	.weak USART4_IRQHandler
	.weak DMA1_Channel8_IRQHandler
	.weak USBFS_IRQHandler
	.weak USBFSWakeUp_IRQHandler
	.weak PIOC_IRQHandler
	.weak OPA_IRQHandler
	.weak USBPD_IRQHandler
	.weak USBPDWakeUp_IRQHandler
	.weak TIM2_CC_IRQHandler
	.weak TIM2_TRG_COM_IRQHandler
	.weak TIM2_BRK_IRQHandler
	.weak TIM3_IRQHandler

NMI_Handler:
HardFault_Handler:
Ecall_M_Mode_Handler:
Ecall_U_Mode_Handler:
Break_Point_Handler:
SysTick_Handler:
SW_Handler:
WWDG_IRQHandler:
PVD_IRQHandler:
FLASH_IRQHandler:
EXTI7_0_IRQHandler:
AWU_IRQHandler:
DMA1_Channel1_IRQHandler:
DMA1_Channel2_IRQHandler:
DMA1_Channel3_IRQHandler:
DMA1_Channel4_IRQHandler:
DMA1_Channel5_IRQHandler:
DMA1_Channel6_IRQHandler:
DMA1_Channel7_IRQHandler:
ADC1_IRQHandler:
I2C1_EV_IRQHandler:
I2C1_ER_IRQHandler:
USART1_IRQHandler:
SPI1_IRQHandler:
TIM1_BRK_IRQHandler:
TIM1_UP_IRQHandler:
TIM1_TRG_COM_IRQHandler:
TIM1_CC_IRQHandler:
TIM2_UP_IRQHandler:
USART2_IRQHandler:
EXTI15_8_IRQHandler:
EXTI25_16_IRQHandler:
USART3_IRQHandler:
USART4_IRQHandler:
DMA1_Channel8_IRQHandler:
USBFS_IRQHandler:
USBFSWakeUp_IRQHandler:
PIOC_IRQHandler:
OPA_IRQHandler:
USBPD_IRQHandler:
USBPDWakeUp_IRQHandler:
TIM2_CC_IRQHandler:
TIM2_TRG_COM_IRQHandler:
TIM2_BRK_IRQHandler:
TIM3_IRQHandler:
infinite_loop:
	j infinite_loop

	.section .text.handle_reset,"ax",@progbits
	.weak handle_reset
	.align 1
handle_reset:
	.option push 
	.option	norelax 
	la gp, __global_pointer$
	.option	pop 
	li sp, _stack

# Configure pipelining and instruction prediction
	li t0, 0x1f
	csrw 0xbc0, t0
# Enable interrupt nesting and hardware stack
	li t0, 0x3
	csrw 0x804, t0
# Enable global interrupt and configure privileged mode
	li t0, 0x88		  
	csrw mstatus, t0
# Configure the interrupt vector table recognition mode and entry address mode
	la t0, _vector_base
	ori t0, t0, 3		
	csrw mtvec, t0

	jal SystemInit

# enable GPIOA port
	li s0,0x40021000 # RCC
	lw a0,24(s0) # RCC->APB2PCENR 
	ori a0,a0,4 # RCC_APB2Periph_GPIOA  0x00000004
	sw a0,24(s0)

# power up the peripherals
	lw a0,28(s0) # RCC->APB1PCENR 
	li a1,0x10000000 # RCC_APB1Periph_PWR  0x10000000
	or a0,a0,a1
	sw a0,28(s0)


# configure PA0
/*
      GPIO_TypeDef :
	CFGLR 0
	CFGHR 4
	INDR 8
	OUTDR 12
	BSHR 16
	BCR 20
	LCKR 24
	CFGXR 28
	BSXR 32
*/
	lui s0,0x40011
	addi s0,s0,-2048 # GPIOA
	lw a0,0(s0)
	# mode: 0=AIN, 4=IN_FLOATING, 5=OUT_OD, 1=OUT_PP, 13=AF_OD, 9=AF_PP
	# 8=IN: BCR->PullDown BSHR->PullUp
	andi a0,a0,0xFFFFFFF0
	ori a0,a0,1 # PA0 : mode OUT_PushPull  50MHz (mode is 4 bits per pin)
	
	sw a0,0(s0) # CFGLR

# main loop
loop1:

# delay 1
	li a0, 0x2FFFFF
loop2:
	addi a0, a0, -1
	bnez a0, loop2

# clear PA0
	li a1,1
	mv a0,s0
	sw a1,20(a0) # BCR

#delay 2
	li a0, 0xFFFFF
loop3:
	addi a0, a0, -1
	bnez a0, loop3

# set PA0
	li a1,1
	mv a0,s0 
	sw a1,16(a0) # BSHR (for PA0-PA7)

#	srli a1,a1,0x10 
#	sw a1,32(a0) # BSXR (for PA8-PA15)

	j loop1


#	la t0, main
#	csrw mepc, t0
#	mret

