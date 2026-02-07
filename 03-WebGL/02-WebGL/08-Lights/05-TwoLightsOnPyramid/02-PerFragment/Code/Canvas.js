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
	AMC_ATTRIBUTE_NORMAL : 2,
};

var shaderProgramObject = null;

var vao_pyramid = null;
var vbo_position_pyramid = null;
var vbo_normal_pyramid = null;

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;

var perspectiveProjectionMatrix;

// parameters to be passed to the shader 
var laUniform = new Array(2);
var ldUniform = new Array(2);
var lsUniform = new Array(2);
var kaUniform = 0;
var kdUniform = 0;
var ksUniform = 0;
var materialShininessUniform = 0;
var lightPositionUniform = new Array(2);
var lKeyPressedUniform = 0;

// rotation angles
var anglePyramid = 0.0;

var bAnimation = false;
var bLight = false;

// Light related variables
class Light {
    constructor() {
        this.ambient = [0.0, 0.0, 0.0, 1.0];
        this.diffuse = [0.0, 0.0, 0.0, 1.0];
        this.specular = [0.0, 0.0, 0.0, 1.0];
        this.position = [100.0, 100.0, 100.0, 1.0];
    }
}

const light = [new Light(), new Light()];

var materialAmbient = [0.0, 0.0, 0.0];
var materialDiffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShininess = 128.0;

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
            case 65:
            case 97:
                // A or a key
                if(bAnimation == false)
                {
                    bAnimation = true;
                    console.log("Animation started");
                }
                else
                {
                    bAnimation = false;
                    console.log("Animation stopped");
                }
                break;
            case 76:
            case 108:
                // L or l key
                if(bLight == false)
                {
                    bLight = true;
                    console.log("Light is ON");
                }
                else
                {
                    bLight = false;
                    console.log("Light is OFF");
                }
                break;
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
        "in vec3 aNormal;\n" +
        "uniform mat4 uModelMatrix;\n" +
        "uniform mat4 uViewMatrix;\n" +
        "uniform mat4 uProjectionMatrix;\n" +
        "uniform vec4 uLightPosition[2];\n" +
        "uniform int uLKeyIsPressed;\n" +
        "out vec3 out_transformedNormal;\n" +
        "out vec3 out_lightDirection[2];\n" +
        "out vec3 out_viewerVector;\n" +
        "void main(void)\n" +
        "{\n" +
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
            "if(uLKeyIsPressed == 1)\n" +
            "{\n" +
                "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
                "mat3 normalMatrix = mat3(uViewMatrix * uModelMatrix);\n" +
                "out_transformedNormal = normalMatrix * aNormal;\n" +
                "out_viewerVector = -eyeCoordinates.xyz;\n" +
                "for(int i = 0; i<2; i++)\n" +
                "{\n" +
                    "out_lightDirection[i] = vec3(uLightPosition[i] - eyeCoordinates);\n" +
                "}\n" +
            "}\n" +
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
        "in vec3 out_transformedNormal;\n" +    
        "in vec3 out_lightDirection[2];\n" +    
        "in vec3 out_viewerVector;\n" + 
        "uniform vec3 uLa[2];\n" +  
        "uniform vec3 uLd[2];\n" +  
        "uniform vec3 uLs[2];\n" +  
        "uniform vec3 uKa;\n" + 
        "uniform vec3 uKd;\n" + 
        "uniform vec3 uKs;\n" + 
        "uniform float uMaterialShininess;\n" +
        "precision highp int;"+ 
        "uniform int uLKeyIsPressed;\n" +   
        "out vec4 fragColor;\n" +   
        "void main(void)\n" +   
        "{\n" + 
            "vec3 phong_ADS_Light;\n" + 
            "vec3 normalizedLightDirection[2];\n" + 
            "vec3 ambientLight[2];\n" + 
            "vec3 diffuseLight[2];\n" + 
            "vec3 reflectionVector[2];\n" + 
            "vec3 specularLight[2];\n" +    
            "if(uLKeyIsPressed == 1)\n" +   
            "{\n" + 
                "vec3 normalizedTransformedNormal = normalize(out_transformedNormal);\n" +  
                "vec3 normalizedViewerVector = normalize(out_viewerVector);\n" +    
                "for(int i = 0; i<2; i++)\n" +  
                "{\n" + 
                    "normalizedLightDirection[i] = normalize(out_lightDirection[i]);\n" +   
                    "ambientLight[i] = uLa[i] * uKa;\n" +   
                    "diffuseLight[i] = uLd[i] * uKd * max(dot(normalizedLightDirection[i], normalizedTransformedNormal), 0.0);\n" + 
                    "reflectionVector[i] = reflect(-normalizedLightDirection[i], normalizedTransformedNormal);\n" + 
                    "specularLight[i] = uLs[i] * uKs * pow(max(dot(reflectionVector[i], normalizedViewerVector), 0.0), uMaterialShininess);\n" +    
                    "phong_ADS_Light = phong_ADS_Light + ambientLight[i] + diffuseLight[i] + specularLight[i];\n" + 
                "}\n" + 
            "}\n" + 
            "else\n" +  
            "{\n" + 
                "phong_ADS_Light = vec3(1.0, 1.0, 1.0);\n" +    
            "}\n" + 
            "fragColor = vec4(phong_ADS_Light, 1.0);\n" +   
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
        gl.bindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_NORMAL, "aNormal");
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
        modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uModelMatrix");
        viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uViewMatrix");
        projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uProjectionMatrix");
        laUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLa[0]");
        ldUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLd[0]");
        lsUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLs[0]");
        lightPositionUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLightPosition[0]");
        laUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLa[1]");
        ldUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLd[1]");
        lsUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLs[1]");
        lightPositionUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLightPosition[1]");
        kaUniform = gl.getUniformLocation(shaderProgramObject, "uKa");
        kdUniform = gl.getUniformLocation(shaderProgramObject, "uKd");
        ksUniform = gl.getUniformLocation(shaderProgramObject, "uKs");
        materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialShininess");
        lKeyPressedUniform = gl.getUniformLocation(shaderProgramObject, "uLKeyIsPressed");

        // Initiliazation of two lights
        // LIGHT 1
        light[0].ambient = [0.0, 0.0, 0.0, 1.0];
        light[0].diffuse = [1.0, 0.0, 0.0, 1.0]; // red
        light[0].specular = [1.0, 0.0, 0.0, 1.0]; // red
        light[0].position = [-2.0, 0.0, 0.0, 1.0];
        // LIGHT 2
        light[1].ambient = [0.0, 0.0, 0.0, 1.0];
        light[1].diffuse = [0.0, 0.0, 1.0, 1.0]; // blue
        light[1].specular = [0.0, 0.0, 1.0, 1.0]; // blue
        light[1].position = [2.0, 0.0, 0.0, 1.0];

        // Provide vertex position, color, normal, texCoord etc.
        var pyramid_position = new Float32Array([
            0.0,  1.0,  0.0, // front-top
            -1.0, -1.0,  1.0, // front-left
            1.0, -1.0,  1.0, // front-right
            
            // right
            0.0,  1.0,  0.0, // right-top
            1.0, -1.0,  1.0, // right-left
            1.0, -1.0, -1.0, // right-right

            // back
            0.0,  1.0,  0.0, // back-top
            1.0, -1.0, -1.0, // back-left
            -1.0, -1.0, -1.0, // back-right

            // left
            0.0,  1.0,  0.0, // left-top
            -1.0, -1.0, -1.0, // left-left
            -1.0, -1.0,  1.0, // left-right
        ]);

        var pyramid_normal = new Float32Array([
            // front
            0.000000, 0.447214,  0.894427, // front-top
            0.000000, 0.447214,  0.894427, // front-left
            0.000000, 0.447214,  0.894427, // front-right
                                    
            // right			    
            0.894427, 0.447214,  0.000000, // right-top
            0.894427, 0.447214,  0.000000, // right-left
            0.894427, 0.447214,  0.000000, // right-right

            // back
            0.000000, 0.447214, -0.894427, // back-top
            0.000000, 0.447214, -0.894427, // back-left
            0.000000, 0.447214, -0.894427, // back-right

            // left
            -0.894427, 0.447214,  0.000000, // left-top
            -0.894427, 0.447214,  0.000000, // left-left
            -0.894427, 0.447214,  0.000000, // left-right
        ]);

        // PYRAMID
        // VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
	    vao_pyramid = gl.createVertexArray();
        // Bind vertex array object
	    gl.bindVertexArray(vao_pyramid);

        // POSITION
        vbo_position_pyramid = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position_pyramid);
        gl.bufferData(gl.ARRAY_BUFFER, pyramid_position, gl.STATIC_DRAW);
        gl.vertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, gl.FALSE, 0, 0);
        gl.enableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_POSITION);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // NORMAL
        vbo_normal_pyramid = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_normal_pyramid);
        gl.bufferData(gl.ARRAY_BUFFER, pyramid_normal, gl.STATIC_DRAW);
        gl.vertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_NORMAL, 3, gl.FLOAT, gl.FALSE, 0, 0);
        gl.enableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_NORMAL);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
        
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
        var modelMatrix = mat4.create(); // this is similar to glLoadIdentity() in display for model view matrix
        var viewMatrix = mat4.create(); // this is similar to glLoadIdentity() in display for view matrix
        var translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]); //translate triangle backwards
        var scaleMatrix = mat4.create();
        mat4.scale(scaleMatrix, scaleMatrix, [0.75, 0.75, 0.75]);
        var rotationMatrix = mat4.create();
        // mat4.rotateX(rotationMatrix, rotationMatrix, Math.PI/180 * anglePyramid);
        mat4.rotateY(rotationMatrix, rotationMatrix, Math.PI/180 * anglePyramid);
        // mat4.rotateZ(rotationMatrix, rotationMatrix, Math.PI/180 * anglePyramid);

        mat4.multiply(modelMatrix, translationMatrix, scaleMatrix);
        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix);

        // send above matrix to the shader in "uniform"
	    gl.uniformMatrix4fv(modelMatrixUniform, gl.FALSE, modelMatrix);
        gl.uniformMatrix4fv(viewMatrixUniform, gl.FALSE, viewMatrix);
	    gl.uniformMatrix4fv(projectionMatrixUniform, gl.FALSE, perspectiveProjectionMatrix);

        if(bLight == true)
        {
            gl.uniform3fv(laUniform[0], light[0].ambient.slice(0,3));
            gl.uniform3fv(ldUniform[0], light[0].diffuse.slice(0,3));
            gl.uniform3fv(lsUniform[0], light[0].specular.slice(0,3));
            gl.uniform4fv(lightPositionUniform[0], light[0].position);

            gl.uniform3fv(laUniform[1], light[1].ambient.slice(0,3));
            gl.uniform3fv(ldUniform[1], light[1].diffuse.slice(0,3));
            gl.uniform3fv(lsUniform[1], light[1].specular.slice(0,3));
            gl.uniform4fv(lightPositionUniform[1], light[1].position);

            gl.uniform3fv(kaUniform, materialAmbient);
            gl.uniform3fv(kdUniform, materialDiffuse);
            gl.uniform3fv(ksUniform, materialSpecular);
            gl.uniform1f(materialShininessUniform, materialShininess);
            gl.uniform1i(lKeyPressedUniform, 1);
        }
        else
        {
            gl.uniform1i(lKeyPressedUniform, 0);
        }

        // Bind with VAO
        gl.bindVertexArray(vao_pyramid);

        // Draw the vertex arrays
        gl.drawArrays(gl.TRIANGLE_FAN, 0, 12);
        
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
        anglePyramid = anglePyramid + 0.01;
        if(anglePyramid >= 360.0)
        {
            anglePyramid = anglePyramid - 360.0;
        }
    }

    function uninitialize()
    {
        // Code
        if(bFullscreen == true)
        {
            toggleFullscreen();
        } 
        
        if(vbo_position_pyramid)
        {
            gl.deleteBuffer(vao_position);
            vbo_position_pyramid = null;
        } 
        
        if(vbo_normal_pyramid)
        {
            gl.deleteBuffer(vao_normal);
            vbo_normal_pyramid = null;
        }

        if(vao_pyramid)
        {
            gl.deleteVertexArray(vao_pyramid);
            vao_pyramid = null;
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


