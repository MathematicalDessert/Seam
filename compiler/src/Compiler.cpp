#include <llvm/Support/CommandLine.h>
#include <llvm/Support/WithColor.h>

int main(int argc, char* argv[]) {
    llvm::raw_ostream &output = llvm::outs();
    llvm::WithColor(output, llvm::raw_ostream::BLUE, false, true) << "text\n";


    
    llvm::cl::ParseCommandLineOptions(argc, argv, "test");
    llvm::cl::opt<std::string> OutputFilename("o", llvm::cl::desc("Specify output filename"), llvm::cl::value_desc("filename"));
}