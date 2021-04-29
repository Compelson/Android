#include <assert.h>
#include <fcntl.h>
#include <gelf.h>
#include <stdio.h>
#include <unistd.h>

void print_dt_needed(const char *elf_path) {
  assert(elf_version(EV_CURRENT) != EV_NONE);

  int fd = open(elf_path, O_RDWR, 0);
  assert(fd >= 0);

  Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
  assert(elf != NULL);
  assert(elf_kind(elf) == ELF_K_ELF);

  Elf_Scn *scn = NULL;
  while ((scn = elf_nextscn(elf, scn)) != NULL) {
    GElf_Shdr shdr = {};
    assert(gelf_getshdr(scn, &shdr) == &shdr);

    if (shdr.sh_type == SHT_DYNAMIC) {
      Elf_Data *data = NULL;
      data = elf_getdata(scn, data);
      assert(data != NULL);

      size_t sh_entsize = gelf_fsize(elf, ELF_T_DYN, 1, EV_CURRENT);

      for (size_t i = 0; i < shdr.sh_size / sh_entsize; i++) {
        GElf_Dyn dyn = {};
        assert(gelf_getdyn(data, i, &dyn) == &dyn);

        if (dyn.d_tag == DT_NEEDED) {
          printf("DT_NEEDED detected: %s\n",
                 elf_strptr(elf, shdr.sh_link, dyn.d_un.d_val));
        }
      }
    }
  }
  assert(elf_end(elf) == 0);
  assert(close(fd) == 0);
}

int main(int argc, char const *argv[]) {
  print_dt_needed(argv[1]);
  return 0;
}