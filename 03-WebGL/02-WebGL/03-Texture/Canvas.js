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
	AMC_ATTRIBUTE_COLOR : 0,
};

var shaderProgramObject = null;

var vao = null;
var vbo_position = null;
var vbo_color = null;

var mvpMatrixUniform;
var perspectiveProjectionMatrix;

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
        "in vec4 aPosition;\n"+
        "uniform mat4 uMVPMatrix;\n"+
        "void main(void)\n"+
        "{\n"+
            "gl_Position = uMVPMatrix * aPosition;\n"+
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
        "out vec4 fragColor;\n"+
        "void main(void)\n"+
        "{\n"+
        "fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"+
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
        gl.bindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_COLOR, "aColor");
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

        // Provide vertex position, color, normal, texCoord etc.
        var triangle_position = new Float32Array([
            0.0, 1.0, 0.0,
            -1.0, -1.0, 0.0,
            1.0, -1.0, 0.0,
        ]);

        // var triangle_color = new Float32Array([
        //     1.0, 0.0, 0.0,
        //     0.0, 1.0, 0.0,
        //     0.0, 0.0, 1.0,
        // ]);
        
        // VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
	    vao = gl.createVertexArray();
        // Bind vertex array object
	    gl.bindVertexArray(vao);

        // POSITION
        vbo_position = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
        gl.bufferData(gl.ARRAY_BUFFER, triangle_position, gl.STATIC_DRAW);
        gl.vertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, gl.FALSE, 0, 0);
        gl.enableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_POSITION);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // // COLOR
        // vbo_color = gl.createBuffer();
        // gl.bindBuffer(gl.ARRAY_BUFFER, vbo_color);
        // gl.bufferData(gl.ARRAY_BUFFER, triangle_color, gl.STATIC_DRAW);
        // gl.vertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_COLOR, 3, gl.FLOAT, gl.FALSE, 0, 0);
        // gl.enableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_COLOR);
        // gl.bindBuffer(gl.ARRAY_BUFFER, null);
        
        // unbind VAO
	    gl.bindVertexArray(null);   

        // Depth related code
        gl.clearDepth(1.0);
        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc(gl.LEQUAL);

        // Clear color
        gl.clearColor(0.0, 0.0, 0.0, 1.0);

        perspectiveProjectionMatrix = mat4.create(); // this is similar to glLoadIdentity() in resize
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
        modelViewMatrix = translationMatrix;
        var modelViewProjectionMatrix = mat4.create();
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        // send above matrix to the shader in "uniform"
	    gl.uniformMatrix4fv(mvpMatrixUniform, gl.FALSE, modelViewProjectionMatrix);

        // Bind with VAO
        gl.bindVertexArray(vao);

        // Draw the vertex arrays
        gl.drawArrays(gl.TRIANGLES, 0, 3);

        // Unbind with VAO
        gl.bindVertexArray(null);

        // unuse shader program object
	    gl.useProgram(null);
        
        // Update call
        update();

        // Double buffering
        request_animation_frame(display, canvas);
    }

    function loadGLTexture(imageFileName)
    {
        // Code
        var tex = gl.createTexture();
        tex.image = new Image();
        tex.image.src = imageFileName;
        tex.image.onLoad = function() // Lambda or clouser
        {
            gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
            gl.bindTexure(gl.TEXTURE_2D, tex);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, tex.image);
            gl.GenerateMipmap(gl.TEXTURE_2D);
            gl.bindTexture(null); //Unbind texture by passing null

            return tex;
        }
    }

    function update()
    {
        // Code
    }

    function uninitialize()
    {
        // Code
        if(bFullscreen == true)
        {
            toggleFullscreen();
        } 
        
        if(vbo_position)
        {
            gl.deleteBuffer(vao_position);
            vbo_position = null;
        } 
        
        if(vbo_color)
        {
            gl.deleteBuffer(vao_color);
            vbo_color = null;
        }

        if(vao)
        {
            gl.deleteVertexArray(vao);
            vao = null;
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


