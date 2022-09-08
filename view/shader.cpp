#include "shader.h"

unsigned int util::load_shader(const char* vertexFilepath, const char* fragmentFilepath) {

	std::ifstream fileReader; // Initialize the File Reader
	std::stringstream bufferedLines; // initialize the String Stream (Allows us to operate on the string)
	std::string line; // Initialize the Line holder variable

	// Read the file and put it into a usable format	
	fileReader.open(vertexFilepath); // Open the File Reader
	while (std::getline(fileReader, line)) { // Iterate through the file line by line, store it in "Line"
		bufferedLines << line << '\n'; // and then put the contents of line into "Buffered Lines"
	}

	// Converts StringStream into C Style String (Because it's C Code stored as .txt for some reason)
	std::string vertexShaderSource = bufferedLines.str(); // Convert StringStream file into a String named "Vertex Shader Source"
	const char* vertexSrc = vertexShaderSource.c_str(); // Converts string into array of characters with nul char at end. Converts to C-Style String
	bufferedLines.str(""); // Deletes data stored in bufferedLines to save memory 
	fileReader.close(); // Closes the fileReader

	// Same as above but for the fragment filepath
	fileReader.open(fragmentFilepath);
	while (std::getline(fileReader, line)) {
		bufferedLines << line << '\n';
	}
	std::string fragmentShaderSource = bufferedLines.str();
	const char* fragmentSrc = fragmentShaderSource.c_str();
	bufferedLines.str("");
	fileReader.close();
	// End of Same as above

	//Unsigned Int is a Non-negative 32-bit integer
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates Vertex Shader Object and stores the index in a 32-Bit non-negative integer
	// Shader, Count, String, Length
	// The "Count" is the number of strings in the array, Which there should be 1
	// If length is "NULL" then the string is assumed to be NULL Terminated (Which it is)
	glShaderSource(vertexShader, 1, &vertexSrc, NULL); // Pass In the Source Code To the Shader
	// Compiles the Shader
	glCompileShader(vertexShader);

	// Just finds and spits out error messages
	int success;
	char errorLog[1024];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 1024, NULL, errorLog);
		std::cout << "Vertex Shader compilation error:\n" << errorLog << '\n';
	}

	// Same as above with the Vertex Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 1024, NULL, errorLog);
		std::cout << "fragment Shader compilation error:\n" << errorLog << '\n';
	}

	// Now link the fragment and vertex shaders together, to make a proper shader (Or in OpenGL, known as a "Program")
	unsigned int shader = glCreateProgram(); // Init Program and return the index so we can reference it
	glAttachShader(shader, vertexShader); // Attach the Vertex Shader to the program
	glAttachShader(shader, fragmentShader); // Attach the Fragment Shader to the program
	glLinkProgram(shader); // Link them together

	// More error checking
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << '\n';
	}
	
	// Delete the original Shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Return the finished shader program
	return shader;
}