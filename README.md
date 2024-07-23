# Win32 Memory Manager
 
This is a custom memory manager designed for real-time C++ applications where memory allocation speed is critical. By providing a low-level interface that directly interacts with the underlying Win32 heap functions, this memory manager bypasses the overhead associated with standard library memory allocation routines, leading to significant performance improvements.

**Key Features**:

- Leverages Win32 heap functions for heap creation and memory allocation.
- Manages all created heaps and provides automatic destruction with memory deallocation.
- Offers two heap types:
  - **Normal Heap:** Grows dynamically and allocates blocks of various sizes with user-specified alignment.
  - **Fixed Heap:** Pre-divided into fixed-size blocks, ideal for allocating constant-size objects like arrays. Offers allocation fail-safes and guarantees 4-byte alignment.
