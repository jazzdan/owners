#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

fs::path owner_path(fs::path p) { return p / "OWNERS"; }

bool has_owner_file(fs::path p) { return fs::exists(owner_path(p)); }

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
    std::ifstream file(owner_path(p));
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        std::cout << line << std::endl;
      }
      file.close();
    }
  } else {
    // eventually we should recurse, but for now let's just error here
    std::cout << "no OWNERS file found" << std::endl;
    return -1;
  }
}