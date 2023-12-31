# Custom Malloc Library Project

## Overview

This project implementation of custom memory allocation functions in C, mirroring the functionality of the standard malloc and free functions. Developed as part of ECE-650 Systems Program & Engineering, this individual project showcases the understanding and application of memory management techniques in a UNIX-based environment, specifically within the Duke Linux system. 

For thread safe implementation check  **`checkpoint-2`:** branch

## Features

### Custom Memory Allocation:
- **`malloc()`:** Custom function for dynamic memory allocation.
- **`free()`:** Custom function to free allocated memory.

### Memory Allocation Strategies:
- **First Fit:** Allocates the first sufficiently large block found.
- **Best Fit:** Chooses the smallest block that fits the request, reducing waste.

### Memory Management Techniques:
- **Coalescing:** Adjacent free memory blocks are merged to prevent fragmentation.
- **Splitting:** Larger free blocks are split to match the allocation request precisely.

### Performance Optimization:
- Minimized data segment size by allocating memory only as needed.
- Efficient free block tracking and retrieval.

### Utility Functions:
- **`get_data_segment_size()`:** Retrieves the size of the data segment.
- **`get_data_segment_free_space_size()`:** Calculates the free space within the data segment.

## Development Environment

- **Platform**: UNIX-based, specifically the Duke Linux environment.
- **Languages & Tools**: C, pthreads, `gdb`, `valgrind`.

## Technical Challenges

- Implementing thread safety in malloc and free operations.
- Applying synchronization mechanisms to manage memory operations.
- Identifying and handling potential race conditions.

## Building and Running

- **Compilation**: Use `Makefile` to build into a shared library `libmymalloc.so`.
- **Usage**: Link with `-lmymalloc` and include `my_malloc.h`.

## Performance Comparison

- Conduct experiments using `thread_test_measurement.c`.
- Assess execution time and allocation efficiency.
- Compare locking and non-locking versions for performance and concurrency trade-offs.

## Submission Contents

- **Report (`report.pdf`)**: Description of thread-safe implementations, synchronization strategies, and performance analysis.

## Contributions

This project was completed as part of an academic assignment with requirments provided requirments.pdf. Contributions were made solely by Koushik Annareddy Sreenath, adhering to the project guidelines and requirements set by the course ECE-650 Systems Program & Engineering

## License

This project is an academic assignment and is subject to university guidelines on academic integrity and software use.

## Acknowledgments

- Thanks to Rabih Younes and the course staff for providing guidance and support throughout the project.


