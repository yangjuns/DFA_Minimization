# DFA_Minimization

HOW TO USE:
  $> make
  $> ./main

(it will print out some experiment results, it should terminate within a minute)

Conclusions:
  1.In my implementations of Moore's and Brzozowski's algorithm, I found out that
    Moore's algorithm is always faster than Brzozowski's.

  2.If we increase the alphabet_size, it's clear that the runtime of Moore's
    algorithm increase linearly. If we increase the number of states, it seems
    like it grows quadratically, which agrees with the runtime O(kn^2).

  3.Brzozowski's suffers from exponential growth of states and alphabet_size.


  /NOTE: ***
  I could not find some examples where Moore's algorithm is slower, no matter on
  specific examples or on average over random DFAs. It might be due to the fact
  that I used bit vector to represent sets of states. If we increase the number
  of states, comparing two sets become more expensive. Moreover, Brzozowski's
  algorithm needs to check the equality of two sets very often. I think that's
  that's the main reason.