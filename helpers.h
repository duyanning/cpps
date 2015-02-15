#ifndef HELPERS_H
#define HELPERS_H

fs::path shadow(fs::path p);
bool can_write_in(fs::path dir);
void move(const fs::path& old_p, const fs::path& new_p);
bool is_a_cpp_src(fs::path f);
bool is_a_c_src(fs::path f);
void safe_remove(fs::path f);

#endif // HELPERS_H
