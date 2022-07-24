# [CProject-077-BCT-HMS](https://github.com/niyoj/CProject-077-BCT-HMS)

## Introduction to the project
This project aims to create a secure database management system for health institutions based on the C programming language, replacing the traditional approach of storing data in hardcopy format. For storing, managing, manipulating, and retrieving the data, concepts of file handling were used, where every data is stored in a specific file. Security of those files was maintained by the implementation of techniques like asymmetric encryption, and security measures like role-based information access control. RSA encryption was used as an asymmetric encryption technique to store personal data and for user verification. A Role-Based Access Control System (RBACS) was implemented to ensure that employees are only granted the necessary level of access to information to perform their job. Employees were grouped into several categories called roles based on the level of information required by their job, thereby providing a secure and efficient way to manage access to the information, while simplifying the process for both the user and the administrator.

## Objective of the project
* To store, manage, manipulate and retrieve the data of health institutions,
* To present the analytics of overall hospitals to the authorized personnel,
* To bring clinical, financial, and administrative departments under one roof.

## Project structure
Every data entered or saved in our program or in the project database is stored in some files inside some directory based on the data. Files and folders denoted by **\*** is supposed not to be edited neither by the user nor by the project adminstrator unless extremly important. Here is a list of core files and folders that you would see in our project. Note that directory name ending with **/** is a folder e.g. `example` is a file while, `example\` is a folder.

* \*`.db/` : This directory is responsible to store all the database contents, environment variables and is supposed not to be modified by user despite of any level of authorization. 
* `bin/` : This directory contains the compiled output of the project and this is what that is run by the  user. It contains executable file for both Windows system `.exe` and Unix system `.out`.
* \*`modules/` : This directory contains files that has some user-defined functions used in our project. 
* `.gitignore` : This file is used to ignore test executable files and swap files.
* `main.c` : This file is the base of our project and contains function protoypes, declarations, includes header files etc.
* `README.md` : This file is for contributors to be read before starting contribution. It covers some brief explanation of our project.

## Before Contributing
Before contributing to the project, you can look at the project's proposal for better understanding about the project, [from this link](https://drive.google.com/file/d/1uvEKRaumQtYOK4uz4cqk8jRVM7G-kg5s/view?usp=sharing). Also, here is the [demo video](https://drive.google.com/file/d/1Bm0yPcZvkcr1tBDbAD2Th6LryI7lrDSG/view?usp=sharing) of the program that will give you the basic idea of what our program really looks like.

## Tips Regarding the Project
### Proper Documentation Style of the Project
Every file created inside must have well defined documentation section in the structure below;
````
/******************************************************************************************************
*   Program name: FILENAME_HERE,
*   Author name: AUTHOR_NAME_HERE, 
*   Created Path: RELATIVE_PATH_OF_THE_FILE_HERE,
*   Created Date: DATE_AND_TIME_IN_THIS_FORMAT_HERE (DD MON YYYY, HH:MM:SS),
*   Description: DECRIPTION_OF_THE_PROJECT_HERE.
*   References: REFRENCES (If Any)
*******************************************************************************************************/
````

## Contributors
This project has recieved major contribution of;
* [Khemraj Shrestha](https://github.com/itsmekhemraj) \(THA077BCT020\)
* [Niyoj Oli](https://github.com/niyoj) \(THA077BCT029\)
* [Om Prakash Sharma](https://github.com/ompiepy) \(THA077BCT030\)
* [Punam Shrestha](https://github.com/punamshrestha1) \(THA077BCT038\)
