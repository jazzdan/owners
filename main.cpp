#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// TODO(dmiller): write some tests maybe

namespace fs = std::filesystem;
using namespace std;

fs::path owners_path(fs::path p) { return p / "OWNERS"; }

bool has_owners_file(fs::path p) { return fs::exists(owners_path(p)); }

void traverse_owners_files(fs::path p, vector<string>& owners) {
  cout << "Checking: " << p << endl;
  if (has_owners_file(p)) {
    auto op = owners_path(p);
    ifstream file(op);
    bool set_noparent = false;
    if (file.is_open()) {
      string line;
      while (getline(file, line)) {
        if (line == "set noparent") {
          set_noparent = true;
        } else {
          owners.push_back(line);
        }
      }
      file.close();
    }

    if (set_noparent) {
      cout << p << "set noparent, not continuing to search for OWNERS files"
           << endl;
      return;
    }
  }

  if (p == p.root_path()) {
    return;
  }

  traverse_owners_files(p.parent_path(), owners);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "must be called with exactly one argument: the path to check "
            "for OWNERS"
         << endl;
    return -1;
  }

  auto path = argv[1];
  fs::path p = path;
  vector<string> owners;
  try {
    traverse_owners_files(fs::absolute(p).parent_path(), owners);
  } catch (const char* e) {
    cout << "Error finding OWNERS file: " << e << endl;
  }

  for (auto& element : owners) {
    cout << element << endl;
  }
}