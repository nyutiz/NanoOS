.section .text.init
.global _start

_start:
    # 1. Configurer la pile (le C ne peut RIEN faire sans pile)
    la sp, _stack_top

    mv s3, a1

    # 2. Nettoyer .bss (mettre les globales non-initialisées à 0)
    la t0, _bss_start
    la t1, _bss_end
    bgeu t0, t1, bss_clear_done

bss_clear_loop:
    sw zero, 0(t0)
    addi t0, t0, 4
    bltu t0, t1, bss_clear_loop

bss_clear_done:
    mv a0, a1
    # 3. Sauter vers le kernel en C
    call kmain

    # 4. Si kernel_main retourne jamais, boucle infinie
hang:
    wfi          # wait for interrupt (économise le CPU)
    j hang