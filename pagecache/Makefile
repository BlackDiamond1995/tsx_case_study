LLVM_PATH = /home/lys/Program/llvm-9.0.0-cfi
LLVM_BIN_PATH = $(LLVM_PATH)/build/bin
CFIHello_So_PATH = $(LLVM_PATH)/build/lib/LLVMCFIHello.so
CC = $(LLVM_PATH)/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign -Xclang -load -Xclang $(CFIHello_So_PATH)
CCA = $(LLVM_PATH)/build/bin/clang

TARGET_NAME = victim

all:
	$(CC) -S $(TARGET_NAME).c -o $(TARGET_NAME).s
	$(CCA) -c $(TARGET_NAME).s -o $(TARGET_NAME).o
	#~/Program/llvm-9.0.0-cfi/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign victim.c -S -o victim.s -Xclang -load -Xclang ~/Program/llvm-9.0.0-cfi/build/lib/LLVMCFIHello.so
	#~/Program/llvm-9.0.0-cfi/build/bin/clang -c victim.s -o victim.o
	clang -c preload.c -o preload.o
	clang -c transactionBegin.s -o transactionBegin.o
	$(CCA) preload.o transactionBegin.o $(TARGET_NAME).o -o $(TARGET_NAME)

clean:
	rm -f $(TARGET_NAME).s $(TARGET_NAME).o preload.o transactionBegin.o $(TARGET_NAME)