FILE(REMOVE_RECURSE
  "CMakeFiles/documentation"
  "html/index.html"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/documentation.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
