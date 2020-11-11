#ifndef BUILTIN_CODEGEN_H
#define BUILTIN_CODEGEN_H

#include "inout.h"
#include "semantic_types.h"

#include <ostream>
#include <filesystem>

void write_builtin_code( const program_resources& pr, const std::filesystem::path& include_path, std::ostream& os ) {
   for( const auto& [name, builtin_overloads] : pr.builtin_overloads ) {
      std::filesystem::path filename( name );
      auto buffer = read_file( include_path / filename.concat( ".h" ) );
      os << buffer.data( ) << "\n";
   }
}

#endif //BUILTIN_CODEGEN_H
