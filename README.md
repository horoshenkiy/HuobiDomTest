# HuobiDomTest

Task was to parse historical log file from Huobi crypto exchange and build DOM (Depth Of The Market).
At each timestamp we should be able to provide top of the book (best bid, best ask)

Measure the resulting performance

I provided two data structures to implement the task
1) standard std::map, which is the easiest way
2) optimized std::vector data structure, to insert new items

Two implementations produce the same output
The comparison between two implementations shows that vector is slightly faster.

