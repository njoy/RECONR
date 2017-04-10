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
4. Delete old `metaconfigure/description.json` file.
5. Generate new `metaconfigure/description.json` file
5. Inspect the new `metaconfigure/description.json`

## License
This software is distributed according to the terms specified in the [LICENSE][1] file.

[1]:	LICENSE
