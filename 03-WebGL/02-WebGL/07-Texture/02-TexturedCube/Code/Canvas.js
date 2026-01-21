var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;
var request_animation_frame = window.requestAnimationFrame ||
                                window.mozRequestAnimationFrame ||
                                window.oRequestAnimationFrame ||
                                window.webKitRequestAnimationFrame ||
                                window.msRequestAnimationFrame;

// WebGL related variables
const MyAttributes = {
    AMC_ATTRIBUTE_POSITION : 0,
	AMC_ATTRIBUTE_COLOR : 1,
	AMC_ATTRIBUTE_TEXCOORD : 2,
};

var shaderProgramObject = null;

var vao_cube = null;
var vbo_position_cube = null;
var vbo_texcoord_cube = null;

var mvpMatrixUniform;
var perspectiveProjectionMatrix;

// rotation angles
var angleCube = 0.0;

// taxture related global variables
var texture_kundali = null;
var textureSamplerUniform = null;

function main()
{
    // Get canvas
    canvas = document.getElementById("AMC")
    if (canvas == null)
    {
        console.log("canvas element cant be optained");
    }
    else{
        console.log("canvas element succesfully obtained");

    }

    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;
    
    // Register our callback fuction as event listeners
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false);

    // Initialize
    initialize();

    // Resize
    resize();

    // Display
    display();

    function keyDown(event)
    {
        // code
        switch(event.keyCode)
        {
            case 70:
            case 102:
                if (bFullscreen == false)
                {
                    toggleFullscreen();
                    bFullscreen = true;  
                }
                else
                {
                    toggleFullscreen();
                    bFullscreen = false;
                }
                break; 
            case 69:
                uninitialize();
                window.close();
                break;
            default:
                break;     
        }
    }

    function mouseDown()
    {
        // code
    }

    function toggleFullscreen()
    {
        // code
        var fullscreen_element = 
        document.fullscreenElement ||
        document.mozFullScreenElement ||
        document.webkitFullscreenElement ||
        document.msFullscreenElement ||
        null;

        if (fullscreen_element == null)
        {
            if (canvas.requestFullscreen)
            {
                canvas.requestFullscreen();

            }
            else if (canvas.mozRequestFullScreen)
            {
                canvas.mozRequestFullScreen();

            }    
            else if (canvas.webkitReuestFullscreen)
            {
                canvas.webkitReuestFullscreen();

            }    
            else if (canvas.msReuestFullscreen)
            {
                canvas.msReuestFullscreen();

            }    

        }
        else
        {
            if(document.exitFullscreen)
            {
                document.exitFullscreen();
            }
            else if(document.mozExitFullScreen)
            {
                document.mozExitFullScreen();
            }
            else if(document.webkitExitFullScreen)
            {
                document.webkitExitFullScreen();
            }
            else if(document.msExitFullScreen)
            {
                document.msExitFullScreen();
            }
        }     
    }

    // Stub Function
    function initialize()
    {
        // Code
        // Get 2D context from canvas
        gl = canvas.getContext("webgl2");
        if (gl == null)
        {
            console.log("Webgl2 Context element cant be optained");
        }
        else
        {
            console.log("Webgl2 Context element succesfully obtained");
        }

        // Set viewport width and viewport height
        gl.viewportWidth = canvas.width;
        gl.viewportHeight = canvas.height;

        // VERTEX SHADER
        // 1. Write shader source code
	    var vertexShaderSourceCode =         
        "#version 300 es\n"+
        "in vec4 aPosition;\n" +
        "in vec2 aTexCoord;\n" +
        "uniform mat4 uMVPMatrix;\n" +
        "out vec2 out_texCoord;\n" +
        "void main(void)\n" +
        "{\n" +
        "gl_Position = uMVPMatrix * aPosition;\n" +
        "out_texCoord = aTexCoord;\n" +
        "}\n";

        // 2. Create the shader object
        var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER)

        // 3. Give the shader source code to the shader object
        gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

        // 4. Compile the shader programmatically
	    gl.compileShader(vertexShaderObject);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(vertexShaderObject);
            if(error.length > 0)
            {
                alert("Error: " + error);
            }   
            
            uninitialize();
        }
        else
        {
            console.log("Vertex shader compilation successfully");
        }

        // FRAGMENT SHADER
        // 1. Write shader source code
        var fragmentShaderSourceCode = 
        "#version 300 es\n"+
        "precision highp float;"+
        "in vec2 out_texCoord;\n" +
        "uniform sampler2D uTextureSampler;\n" +
        "out vec4 fragTexCoord;\n" +
        "void main(void)\n" +
        "{\n" +
        "fragTexCoord = texture(uTextureSampler, out_texCoord);\n" +
        "}\n";

        // 2. Create the shader object
        var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

        // 3. Give the shader source code to the shader object
        gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

        // 4. Compile the shader programmatically
        gl.compileShader(fragmentShaderObject);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(fragmentShaderObject);
            if(error.length > 0)
            {
                alert("Error: " + error);
            }   
            
            uninitialize();
        }
        else
        {
            console.log("Fragment shader compilation successfully");
        }

        // Create, attach, link shader program object
        shaderProgramObject = gl.createProgram();
        gl.attachShader(shaderProgramObject, vertexShaderObject);
        gl.attachShader(shaderProgramObject, fragmentShaderObject);

        // Bind shader attribute at a certain index in shader to save index in host program
        gl.bindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_POSITION, "aPosition");
        gl.bindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
        gl.linkProgram(shaderProgramObject);

        if(gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false)
        {
            var error = gl.getProgramInfoLog(shaderProgramObject);
            if(error.length > 0)
            {
                alert("Error: " + error);
            }   
            
            uninitialize();
        }
        else
        {
            console.log("Shader linking successfully");
        }

        // Get the required uniform location from the shader
        mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");
        textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "uTextureSampler");

        // Provide vertex position, color, normal, texCoord etc.
        var cube_position = new Float32Array([
        // front
		1.0,  1.0,  1.0, // top-right of front
		-1.0,  1.0,  1.0, // top-left of front
		-1.0, -1.0,  1.0, // bottom-left of front
		1.0, -1.0,  1.0, // bottom-right of front

		// right
		1.0,  1.0, -1.0, // top-right of right
		1.0,  1.0,  1.0, // top-left of right
		1.0, -1.0,  1.0, // bottom-left of right
		1.0, -1.0, -1.0, // bottom-right of right

		// back
		1.0,  1.0, -1.0, // top-right of back
		-1.0,  1.0, -1.0, // top-left of back
		-1.0, -1.0, -1.0, // bottom-left of back
		1.0, -1.0, -1.0, // bottom-right of back

		// left
		-1.0,  1.0,  1.0, // top-right of left
		-1.0,  1.0, -1.0, // top-left of left
		-1.0, -1.0, -1.0, // bottom-left of left
		-1.0, -1.0,  1.0, // bottom-right of left

		// top
		1.0,  1.0, -1.0, // top-right of top
		-1.0,  1.0, -1.0, // top-left of top
		-1.0,  1.0,  1.0, // bottom-left of top
		1.0,  1.0,  1.0, // bottom-right of top

		// bottom
		1.0, -1.0,  1.0, // top-right of bottom
		-1.0, -1.0,  1.0, // top-left of bottom
		-1.0, -1.0, -1.0, // bottom-left of bottom
		1.0, -1.0, -1.0, // bottom-right of bottom
	
        ]);

        var cube_texcoord = new Float32Array([
            // front
		1.0, 1.0, // top-right of front
		0.0, 1.0, // top-left of front
		0.0, 0.0, // bottom-left of front
		1.0, 0.0, // bottom-right of front

		// right
		1.0, 1.0, // top-right of right
		0.0, 1.0, // top-left of right
		0.0, 0.0, // bottom-left of right
		1.0, 0.0, // bottom-right of right

		// back
		1.0, 1.0, // top-right of back
		0.0, 1.0, // top-left of back
		0.0, 0.0, // bottom-left of back
		1.0, 0.0, // bottom-right of back

		// left
		1.0, 1.0, // top-right of left
		0.0, 1.0, // top-left of left
		0.0, 0.0, // bottom-left of left
		1.0, 0.0, // bottom-right of left

		// top
		1.0, 1.0, // top-right of top
		0.0, 1.0, // top-left of top
		0.0, 0.0, // bottom-left of top
		1.0, 0.0, // bottom-right of top

		// bottom
		1.0, 1.0, // top-right of bottom
		0.0, 1.0, // top-left of bottom
		0.0, 0.0, // bottom-left of bottom
		1.0, 0.0, // bottom-right of bottom
        ]);

        // CUBE
        // VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
	    vao_cube = gl.createVertexArray();
        // Bind vertex array object
	    gl.bindVertexArray(vao_cube);

        // POSITION
        vbo_position_cube = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_cube);
        gl.bufferData(gl.ARRAY_BUFFER, cube_position, gl.STATIC_DRAW);
        gl.vertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, gl.FALSE, 0, 0);
        gl.enableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_POSITION);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // TEXCOORD
        vbo_texcoord_cube = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texcoord_cube);
        gl.bufferData(gl.ARRAY_BUFFER, cube_texcoord, gl.STATIC_DRAW);
        gl.vertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_TEXCOORD, 2, gl.FLOAT, gl.FALSE, 0, 0);
        gl.enableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_TEXCOORD);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
        
        // unbind VAO
	    gl.bindVertexArray(null);   

        // Depth related code
        gl.clearDepth(1.0);
        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc(gl.LEQUAL);

        // Clear color
        gl.clearColor(0.0, 0.0, 0.0, 1.0);

        // Load textures
        texture_kundali = loadGLTexture("Vijay_Kundali.png");
        if(!texture_kundali)
        {
            console.log("texture loading failed");
            uninitialize();
        }

        perspectiveProjectionMatrix = mat4.create(); // this is similar to glLoadIdentity() in resize
    }

    function loadGLTexture(imageFileName)
    {
        // code
        var texture = gl.createTexture();
        texture.image = new Image();
        texture.image.src = imageFileName;
        
        texture.image.onload = function()
        {
            gl.bindTexture(gl.TEXTURE_2D, texture);
            gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture.image);
            gl.generateMipmap(gl.TEXTURE_2D);
            gl.bindTexture(gl.TEXTURE_2D, null);
        };
        return texture;
    }


    function resize()
    {
        // Code
        if(bFullscreen == true)
        {
            canvas.width = window.innerWidth;
            canvas.height = window.innerHeight;
        }
        else
        {
            canvas.width = canvas_original_width;
            canvas.height = canvas_original_height;
        }

        gl.viewport(0, 0, canvas.width, canvas.height);

        // do perspective projection
	    perspectiveProjectionMatrix = mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width)/parseFloat(canvas.height), 0.1, 100.0);
    }

    

    function display()
    {
        // Code
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        // use shader program object
	    gl.useProgram(shaderProgramObject);

        // Transformations
        var modelViewMatrix = mat4.create(); // this is similar to glLoadIdentity() in display for model view matrix
        var translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]); //translate triangle backwards
        var rotationMatrix = mat4.create();
        mat4.rotate(rotationMatrix, rotationMatrix, angleCube, [0.0, 1.0, 0.0]);
        mat4.multiply(modelViewMatrix, translationMatrix, rotationMatrix);
        var modelViewProjectionMatrix = mat4.create();
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
        
        // send above matrix to the shader in "uniform"
	    gl.uniformMatrix4fv(mvpMatrixUniform, gl.FALSE, modelViewProjectionMatrix);

        // bind texture
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, texture_kundali);
        gl.uniform1i(textureSamplerUniform, 0);

        // Bind with VAO
        gl.bindVertexArray(vao_cube);

        // Draw the vertex arrays
        gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);

        // unbind texture                           
        gl.bindTexture(gl.TEXTURE_2D, null);
        
        // Unbind with VAO
        gl.bindVertexArray(null);

        // unuse shader program object
	    gl.useProgram(null);
        
        // Update call
        update();

        // Double buffering
        request_animation_frame(display, canvas);
    }

    function update()
    {
        // Code
        angleCube = angleCube + 0.01;
        if(angleCube >= 360.0)
        {
            angleCube = angleCube - 360.0;
        }
    }

    function uninitialize()
    {
        // Code
        if(bFullscreen == true)
        {
            toggleFullscreen();
        } 
        
        if(vbo_position_cube)
        {
            gl.deleteBuffer(vao_position);
            vbo_position_cube = null;
        } 
        
        if(vbo_texcoord_cube)
        {
            gl.deleteBuffer(vbo_texcoord_cube);
            vbo_texcoord_cube = null;
        }

        if(vao_cube)
        {
            gl.deleteVertexArray(vao_cube);
            vao_cube = null;
        }

        if(shaderProgramObject)
        {
            gl.useProgram(shaderProgramObject);
            var shaderObjects = gl.getAttachedShaders(shaderProgramObject);
            for(let i = 0; i < shaderObjects.length; i++)
            {
                gl.detachShader(shaderProgramObject, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null;
            } 
            gl.useProgram(null);
            gl.deleteProgram(shaderProgramObject);
            shaderProgramObject = null;  
            
        
        }    
    }



}


