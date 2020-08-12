#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool has_owner_file(fs::path p) { return fs::exists(p / "OWNERS"); }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "must be called with exactly one argument: the path to check "
                 "for OWNERS"
              << std::endl;
    return -1;
  }

  auto path = argv[1];
  fs::path p = path;
  std::cout << "Checking: " << p << std::endl;
  auto has_file = has_owner_file(p);
  if (has_file) {
    // TODO read the file
  } else {
    std::cout << "no OWNERS file found" << std::endl;
    return -1;
  }

  // eventually we should recurse, but for now let's just error here
}