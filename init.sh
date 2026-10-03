#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <new-project-name>"
  exit 1
fi

name="$1"
upper="${name^^}"
lower="${name,,}"

if [[ ! -f CMakeLists.txt ]]; then
  echo "Run this script from the repository root."
  exit 1
fi

root="$(pwd)"

replace_in_files() {
  local from="$1"
  local to="$2"
  grep -rlI "${from}" "$root" \
    --exclude-dir=.git --exclude-dir=build --exclude-dir=install | while read -r file; do
    sed -i "s/${from}/${to}/g" "$file"
  done
}

replace_in_files "pkg" "${lower}"
replace_in_files "PKG" "${upper}"

# Rename directories
mv "include/pkg" "include/${lower}"

# Rename files with pkg in the name (avoiding generated directories)
find "$root" -name '*pkg*' \
  -not -path '*/.git/*' \
  -not -path '*/build/*' \
  -not -path '*/install/*' \
  -print0 | while IFS= read -r -d '' file; do
  newname="${file//pkg/${lower}}"
  if [[ "$file" != "$newname" ]]; then
    mv "$file" "$newname"
  fi
done

# Self-destruct
rm -- "$0"

echo "Initialized project '${name}' (namespace ${lower}::, macros ${upper}_*)."
