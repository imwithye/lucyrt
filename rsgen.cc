// Copyright 2019
#include <stdio.h>
#include <iostream>
#include <string>

using std::string;

void replace_all(string *str, const string from, const string to) {
  size_t start_pos;
  while ((start_pos = str->find(from)) != std::string::npos) {
    str->replace(start_pos, from.length(), to);
  }
}

int main(int argc, const char **argv) {
  string output(argv[1]);
  string workdir(argv[2]);
  printf("\033[0;32m[%3d%%] RSGEN::OUTPUT  --> %s\n\033[0m", 0, output.c_str());
  printf("\033[0;32m[%3d%%] RSGEN::WORKDIR --> %s\n\033[0m", 0,
         workdir.c_str());
  FILE *out = fopen(output.c_str(), "w");
  fprintf(out, "// Copyright 2019\n");
  fprintf(out, "#pragma once\n\n");
  fprintf(out, "#include <stdlib.h>\n");
  for (int i = 3; i < argc; i++) {
    int progress = static_cast<int>((i / (static_cast<float>(argc - 1))) * 100);
    printf("\033[0;32m[%3d%%] RSGEN::GEN %s\n\033[0m", progress, argv[i]);
    string filepath(argv[i]), filesym(argv[i]), empty;
    filesym.replace(filesym.find(workdir), workdir.size() + 1, "");
    replace_all(&filesym, ".", "_");
    replace_all(&filesym, "-", "_");
    replace_all(&filesym, "/", "_");
    replace_all(&filesym, "\\", "_");
    FILE *in = fopen(filepath.c_str(), "r");
    fprintf(out, "const char %s[] = {\n", filesym.c_str());
    unsigned char buf[256];
    size_t nread = 0;
    size_t linecount = 0;
    do {
      nread = fread(buf, 1, sizeof(buf), in);
      size_t i;
      for (i = 0; i < nread; i++) {
        fprintf(out, "0x%02x, ", buf[i]);
        if (++linecount == 10) {
          fprintf(out, "\n");
          linecount = 0;
        }
      }
    } while (nread > 0);
    if (linecount > 0) {
      fprintf(out, "\n");
    }
    fprintf(out, "'\\0'};\n");
    fprintf(out, "const size_t %s_len = sizeof(%s) - 1;\n\n", filesym.c_str(),
            filesym.c_str());
    fclose(in);
  }
  fclose(out);
  printf("\033[1;32m[%3d%%] RSGEN::DONE\n\033[0m", 100);
  return 0;
}
