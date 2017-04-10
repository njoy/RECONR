# NJOY metaconfigure
The metaconfigure package contains a suite of Python scripts to assist in the generation of build system configuration files for NJOY projects.

## Updating metaconfigure in a project
**Note:** that all the subprojects/dependencies must be updated prior to updating the super project.

These commands (unless otherwise specified) should be run from the top level of a project.
1. Create a branch
2. Download ("fetch") subprojects
   ```bash
   python metaconfigure/fetch_subprojects.py
   ```
3. Update the metaconfigure subtree
   ```bash
   git subtree pull --squash --prefix metaconfigure https://github.com/njoy/metaconfigure.git master
   ```
4. Delete old `description.json` file.
   ```bash
   rm metaconfigure/description.json
   ```
5. Generate new `metaconfigure/description.json` file. This is done in Python.
   ```python
   import metaconfigure
   args = {"name": "project-name", "language": "c++", "is external project": False, "include path": "src/include"}
   metaconfigure.description.generate(**args)
   ```
   Note that the values in `args` need to be appropriate for your particular project. The values given above are just an example.
6. Inspect the new `metaconfigure/description.json`. In particular check:
	- `implementation files`—ensure the correct relative path (to top-level of project) is correct.
	- `header files`—ensure the correct relative path (to top-level of project) is correct.
	- `include path`—ensure that this points to the correct path. This is needed for any C++ project.
7. Generate a [CMake](https://cmake.org) file
   ```bash
   metaconfigure/generate.py cmake
   ```
8. Inspect resulting `CMakelists.txt` file
	 - Is the target of the expected type (executable, interface, or library)?
	 - Are the tests specified?
	 - Are the compiler flags specified?
9. Build/test project locally to make sure everything works.
10. Update `.travis.yml` and `.travis.sh`.
11. Push to GitHub.
12. Create pull request.


## License
This software is distributed according to the terms specified in the [LICENSE](LICENSE) file.
