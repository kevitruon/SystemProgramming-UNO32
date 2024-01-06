Kevin Truong (1834510)
ktruon13@ucsc.edu
Collaborated With: N/A

**Results:**
SelectionSort() = 47.334ms
InsertionSort() = 32.345ms

I believe InsertionSort() is faster because, it only scans the LinkedList as much as it needs to. On the other hand, SelectionSort() was slower because it iterates through
all of the data inside of the LinkedList.

**Summary:**
What I found to be the most important aspect of this lab is that we had to draw out the "nodes" on a piece of paper and make them point to each other. Then we use those steps 
and write it out into code. I think this is very important practice because in the future we have to write out algorithms in order ensure we have a working function.

**Approach:**
My approach to the lab was to research what strcmp(), malloc(), and free(), did because they were very crucial to our functions in LinkedList.c and Sort.c. This lab was very hard because
in the end, my InsertionSort() would print out at all. What I had realise it that a couple lines before was a CreateUnsortedList() and tested it if it works twice. Later, after 
scrolling desperately through the class discord was that I needed to allocate a larger heap size. Before my heap size was 2048 bytes, but after changing it to 4096 bytes, my code
worked wonders.

**Thoughts?:**
I spent about 2-3 implementing all the functions and creating the test harness for LinkedList. In the lab manual, I think it should've explained that we may need to allocate alot 
of heaps to ensure that our code runs properly. The heap size issue caused me to be stuck for 5-6 hours changing my code back and forth realizing that it won't work at all.
