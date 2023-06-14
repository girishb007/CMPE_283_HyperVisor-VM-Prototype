    .text
    .globl main
main:
    # Load immediate values into registers
    li      $1, 6
    li      $2, 9
    li      $3, 12

    # Perform arithmetic operations
    add     $4, $1, $2     # $4 = $1 + $2
    sub     $5, $2, $1     # $5 = $2 - $1
    addi    $6, $3, 15     # $6 = $3 + 15
    mul     $7, $4, $5     # $7 = $4 * $5
    and     $8, $2, $3     # $8 = $2 & $3
    or      $9, $1, $4     # $9 = $1 | $4
    xor     $10, $6, $8    # $10 = $6 ^ $8
    or      $11, $4, 100   # $11 = $4 | 100
    sll     $12, $7, 10    # $12 = $7 << 10
    srl     $13, $8, 10    # $13 = $8 >> 10

    # Dump register values
    li      $v0, 34           # system call code for print integer
    addu    $a0, $zero, $1    # load $1 into $a0
    syscall                   # print $1
    addi    $a0, $zero, 0xA   # load newline character into $a0
    li      $v0, 11           # system call code for print character
    syscall                   # print newline
    addu    $a0, $zero, $2    # load $2 into $a0
    li      $v0, 34           # system call code for print integer
    syscall                   # print $2
    addi    $a0, $zero, 0xA   # load newline character into $a0
    li      $v0, 11           # system call code for print character
    syscall                   # print newline
    addu    $a0, $zero, $3    # load $3 into $a0
    li      $v0, 34           # system call code for print integer
    syscall                   # print $3
    addi    $a0, $zero, 0xA   # load newline character into $a0
    li      $v0, 11           # system call code for print character
    syscall                   # print newline
    addu    $a0, $zero, $4    # load $4 into $a0
    li      $v0, 34           # system call code for print integer
    syscall                   # print $4
    addi    $a0, $zero, 0xA   # load newline character into $a0
    li      $v0, 11           # system call code for print character
    syscall                   # print newline
    addu    $a0, $zero, $5    # load $5 into $a0
    li      $v0, 34           # system call code for print integer
    syscall                   # print $5
    addi    $a0, $zero, 0xA   # load newline character into $a0
    li      $v
