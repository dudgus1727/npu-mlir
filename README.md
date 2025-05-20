### Clone and Initialize

```
$ git clone git@github.com:npu-compiler/npu-mlir.git
$ cd npu-mlir
$ git submodule update --init
```

### Build and Test LLVM/MLIR/CLANG
```
$ cd npu-mlir
$ mkdir llvm/build
$ cd llvm/build
$ cmake -G Ninja ../llvm \
    -DLLVM_ENABLE_PROJECTS="mlir;clang" \
    -DLLVM_TARGETS_TO_BUILD="host;RISCV" \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DCMAKE_BUILD_TYPE=RELEASE
$ ninja check-mlir check-clang
```

### Build npu-mlir

If you have previously built the llvm-project, you can replace the $PWD with the path to the directory where you have successfully built the llvm-project.

```
$ cd npu-mlir
$ mkdir build
$ cd build
$ cmake -G Ninja .. \
    -DMLIR_DIR=$PWD/../llvm/build/lib/cmake/mlir \
    -DLLVM_DIR=$PWD/../llvm/build/lib/cmake/llvm \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DCMAKE_BUILD_TYPE=RELEASE
$ ninja
$ ninja check-npu
```