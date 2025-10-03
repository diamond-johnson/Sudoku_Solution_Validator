# Parallel Sudoku Validator Using Pthreads

This project implements a multithreaded Sudoku validator in C. It uses POSIX threads to parallelize the validation of a 9x9 Sudoku grid by checking rows, columns, and 3x3 subgrids concurrently for unique numbers from 1 to 9.

## Features
- **Parallel Validation**: 27 threads (9 for rows, 9 for columns, 9 for subgrids) run simultaneously to verify Sudoku rules.
- **Fixed Grid Size**: Hardcoded 9x9 Sudoku puzzle for simplicity.
- **Thread Synchronization**: Main thread waits for all validation threads to complete before deciding on validity.
- **Early Exit in Threads**: Invalid checks exit early to avoid unnecessary computation.
- **Result Aggregation**: Uses a shared `results` array to collect outcomes from each thread.

## How It Works
1. **Thread Creation**:
   - **Rows (9 threads)**: Each thread checks one row for unique 1-9.
   - **Columns (9 threads)**: Each thread checks one column for unique 1-9.
   - **Subgrids (9 threads)**: Each thread checks one 3x3 subgrid (starting at indices like (0,0), (0,3), etc.) for unique 1-9.

2. **Validation Logic**:
   - Uses a boolean array (`nums[10]`) to track seen numbers.
   - If a number is out of 1-9 range or duplicate, sets `results[index] = 0` and exits.
   - Otherwise, sets `results[index] = 1`.

3. **Synchronization and Result Check**:
   - Main thread joins all 27 threads using `pthread_join`.
   - Scans `results` array; if any is 0, Sudoku is invalid.

Note: This implementation assumes a solved Sudoku (no empty cells). It does not solve puzzles—only validates given grids. Shared `results` array and `sudoku` grid are accessed without locks, relying on read-only access per thread (safe for this use case).

## Building and Running
### Prerequisites
- A Unix-like system (Linux, macOS) or Windows with MinGW/Cygwin.
- GCC or compatible C compiler.
- Pthreads support (enabled by default on Unix; use `-pthread` flag).

### Compilation
```bash
gcc -o sudoku_validator -pthread main.c
```
### Execution
```bash
./sudoku_validator
```
### Example Output
```bash
Sudoku IS valid!
```
