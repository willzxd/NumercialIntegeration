#Mini-Project: Numerical Methods
#### Do not copy the code for your homework

### Parsing functions

Input:

	define  (f x y z) = (+ (* x y) (- z 3))
	evaluate (f 3 4.2 5)
	evaluate (f -1 9.2 6)
  
Output:

	defined  (f x y z)
	(f 3 4.2 5) = 14.6
	(f -1 9.2 6) = -6.2

Specifically, define must be followed by (id id ...)  (that is, parenthesis containing one or more "id"s---which are the parameters to this function), where "id" is short for identifier, and for these purposes, an identifier is any combination of characters other than whitespace or numbers.   	
Next must be an = sign, followed by an expression.  An expression is either (id expression ...) [that is, an id followed by one or more expressions],OR an id by itself, or a numeric constant.  The following are all valid expressions:
    
    (+ 3 (* x y))
    (f x y zqweq (g (*7 n) 3))
    x
    -4.2
    23
    
All function names (the first identifier in the parenthesis) must refer to a previously defined function, and all other identifiers must refer to parameters to this function.  It is an error to attempt to re-define a function which is already defined.

Evaluate commands must have an expression with no variables---they may refer to functions, and have numeric constants for the parameters.  However, the may call other functions.  The following are all valid (assuming appropriate definitions of f, g, and h exist):

	(f 3 4.2 5)
	(f (g 1 2) (g 3 4) (h 5 6 7))

###Numerical Integration

support for numerical integration, using the midpoint-rectangle method.
command "numint" (short for "numerical integration").

* The first argument will always be the name of a defined function
* The second argument will be the "stepsize" (that is the width of each rectangle).
* The remaining arguments come in pairs, specifying the range of integration for each parameter. There must be one range (pair of numbers) for each parameter.

	define  (f x y) = (+ (* 2 x) y)
	numint f 0.001 -1 1 4 5

This would compute the volume under the curve of f(x,y) = 2*x + y with x ranging from -1 to 1, and y ranging from 4 to 5.   It would compute this by summing the volumes of 2,000,000 rectangular prisms, each of which has length 0.001, and width 0.001, and a height based on the value of f at the center of that particular rectangle.

###Monte Carlo Integration
The arguments to "mcint" are the same as "numint" except that the second argument is the number of trials (the number of random points to pick) instead of the step size.  The first parameter is still the function name, and the remaining parmaters are still the ranges to integrate over.
 

###Gradient Ascent

The commands should take the following arguments:
  1. The name of the function
  2. gamma
  3. convergedDistance
  4. The start point, which will have as many numbers as the function has
     parameters.

      
      
