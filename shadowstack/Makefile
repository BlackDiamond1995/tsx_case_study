LLVM_PATH = /home/lys/Program/llvm-9.0.0-cfi
LLVM_BIN_PATH = $(LLVM_PATH)/build/bin
CFIHello_So_PATH = $(LLVM_PATH)/build/lib/LLVMCFIHello.so
CC = $(LLVM_PATH)/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign -Xclang -load -Xclang $(CFIHello_So_PATH)
CCA = $(LLVM_PATH)/build/bin/clang

TARGET_NAME = foo

all:
	$(CC) -S $(TARGET_NAME).c -o $(TARGET_NAME).s
	$(CCA) -c $(TARGET_NAME).s -o $(TARGET_NAME).o
	# ~/Program/llvm-9.0.0-cfi/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign foo.c -S -o foo.s -Xclang -load -Xclang ~/Program/llvm-9.0.0-cfi/build/lib/LLVMCFIHello.so
	# ~/Program/llvm-9.0.0-cfi/build/bin/clang -c foo.s -o foo.o
	clang -c transactionBegin.s -o transactionBegin.o
	@fallocate -l 4M ssblob
	@objcopy --add-section .ssblob=ssblob --set-section-flags .ssblob=alloc,data,code $(TARGET_NAME).o
	# ~/Program/llvm-9.0.0-cfi/build/bin/clang foo.o transactionBegin.o foo.lds -o foo
	$(CCA) $(TARGET_NAME).o transactionBegin.o shadowstack.lds -o $(TARGET_NAME)

clean:
	rm -f $(TARGET_NAME).s $(TARGET_NAME).o transactionBegin.o ssblob $(TARGET_NAME)