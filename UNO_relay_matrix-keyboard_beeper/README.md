# UNO_relay_matrix-keyboard_beeper
  + Code lock on UNO + relay + matrix keyboard + beeper
  + When loading the sketch to the board, the default password is 0000
  
### Code lock on UNO + relay + matrix keyboard + sounder
  + To change the password, enter ABCD (a signal will sound) and the previous password, after a sound signal, enter a new password, after entering each digit of a new password, there should be a signal, upon completion of entering a new password, a long signal will sound and the new password will be stored in the non-volatile memory
  + If you enter the wrong password, a low signal will sound, with each new attempt to enter the wrong password, the signal will become longer and after 4 incorrect attempts, the lock will block entering the password for 30 seconds.

## Elements
  + Signal - D11
  + Relay
    + In1 - D12
    + VCC - 5v
  + Keyboard
    + Rows - D5, D4, D3, D2
    + Columns - D9, D8, D7, D6
