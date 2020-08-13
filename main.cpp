#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

ABSL_FLAG(bool, should_debug, false, "Whether or not to print debug logs");

// TODO(dmiller): write some tests maybe

namespace fs = std::filesystem;
using namespace std;

fs::path owners_path(fs::path p) { return p / "OWNERS"; }

bool has_owners_file(fs::path p) { return fs::exists(owners_path(p)); }

void debug_log(std::string m) {
  if (absl::GetFlag(FLAGS_should_debug)) {
    cout << m << endl;
  }
}

void traverse_owners_files(fs::path p, vector<string>& owners) {
  debug_log("Checking: " + p.string());
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
      debug_log(p.string() +
                " set noparent, not continuing to search for OWNERS files");
      return;
    }
  }

  if (p == p.root_path()) {
    return;
  }

  traverse_owners_files(p.parent_path(), owners);
}

int main(int argc, char* argv[]) {
  absl::ParseCommandLine(argc, argv);
  if (argc < 2) {
    cout << "must be called with at least one argument: the path to check "
            "for OWNERS"
         << endl;
    return -1;
  }

  auto path = argv[1];
  fs::path p = path;
  vector<string> owners;
  traverse_owners_files(fs::absolute(p).parent_path(), owners);

  for (auto& element : owners) {
    cout << element << endl;
  }
}