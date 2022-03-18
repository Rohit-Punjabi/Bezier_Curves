# Bezier_Curves
An academic project for the MAT500: Curves and Surfaces class at DigiPen Institute of Technology. Created an application that generates Bezier Curves based on control points that places by the user.
#
The Curves are calculated using the following three methods:

1. De-Casteljau algorithm or Nested Linear Interpolation (NLI)
2. Bernstein Polynomials (BB-Form)
3. Midpoint Subdivision
#
Made the application using Qt 5.15.0 and qcustomplot library
#
Project file and source code are in "SourceCode" Folder

To open project:

SourceCode > Mat_500_Project_2.pro
#
To run executable:

release > Mat_500_Project_2.exe
#
Instructions to use application:

Left Click to add a new point Right click on a point and drag to drag a point. Use the Drop-Down menu to select calculation method. Use the Checkbox to toggle display of shell points - only works when "NLI" is selected. Drag the Horizontal Slider to adjust the t value (0-1) - only works when "NLI" is selected.
