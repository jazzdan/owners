#include <filesystem>
#include <fstream>
#include <iostream>

// TODO(dmiller): combine all recursively
// TODO(dmiller): support `set noparent`
// TODO(dmiller): write some tests maybe

namespace fs = std::filesystem;

fs::path owner_path(fs::path p) { return p / "OWNERS"; }

bool has_owner_file(fs::path p) { return fs::exists(owner_path(p)); }

fs::path find_owners_file_path(fs::path p) {
  std::cout << "Checking: " << p << std::endl;
  if (has_owner_file(p)) {
    return owner_path(p);
  }

  if (p == p.root_path()) {
    throw "reached root path and no OWNERS file was found";
  }

  return find_owners_file_path(p.parent_path());
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "must be called with exactly one argument: the path to check "
                 "for OWNERS"
              << std::endl;
    return -1;
  }

  auto path = argv[1];
  fs::path p = path;
  try {
    auto owners_path = find_owners_file_path(fs::absolute(p));
    std::ifstream file(owner_path(p));
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        std::cout << line << std::endl;
      }
      file.close();
    }
  } catch (const char* e) {
    std::cout << "Error finding OWNERS file: " << e << std::endl;
  }
}