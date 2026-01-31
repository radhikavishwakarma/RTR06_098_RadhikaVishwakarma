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

var shaderProgramObject_pv = null;
var shaderProgramObject_pf = null;

var sphere = null;

var modelMatrixUniform_pv;
var viewMatrixUniform_pv;
var projectionMatrixUniform_pv;
var modelMatrixUniform_pf;
var viewMatrixUniform_pf;
var projectionMatrixUniform_pf;

var perspectiveProjectionMatrix;

// parameters to be passed to the shader 
var laUniform_pv = 0;
var ldUniform_pv = 0;
var lsUniform_pv = 0;
var kaUniform_pv = 0;
var kdUniform_pv = 0;
var ksUniform_pv = 0;
var materialShininessUniform_pv = 0;
var lightPositionUniform_pv = 0;
var lKeyPressedUniform_pv = 0;

var laUniform_pf = 0;
var ldUniform_pf = 0;
var lsUniform_pf = 0;
var kaUniform_pf = 0;
var kdUniform_pf = 0;
var ksUniform_pf = 0;
var materialShininessUniform_pf = 0;
var lightPositionUniform_pf = 0;
var lKeyPressedUniform_pf = 0;

// rotation angles
var angleSphere = 0.0;

var bAnimation = false;
var bLight = false;
var bPerVertex = true;
var bPerFragment = false;

// Light related variables
var lightAmbient = [0.0, 0.0, 0.0];
var lightDiffuse = [1.0, 1.0, 1.0];
var lightSpecular = [1.0, 1.0, 1.0];
var lightPosition = [100.0, 100.0, 100.0, 1.0];
var materialAmbient = [0.0, 0.0, 0.0];
var materialDiffuse = [0.5, 0.2, 0.7];
var materialSpecular = [0.7, 0.7, 0.7];
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
            case 65:  // A or a key
            case 97:
                if (bAnimation == false)
                {
                    bAnimation = true;
                    console.log("Animation started\n");
                }
                else
                {
                    bAnimation = false;
                    console.log("Animation stopped\n");
                }
                break;
            case 76:  // L or l key
            case 108:
                if (bLight == false)
                {
                    bLight = true;
                    console.log("Lighting enabled\n");
                }
                else
                {
                    bLight = false;
                    console.log("Lighting disabled\n");
                }
                break;
            case 70:  // F or f key
            case 102:
                bPerFragment = true;
                bPerVertex = false;
                console.log("Switched to Per-Fragment lighting\n");
                break;
            case 86:  // V or v key for per-vertex/per-fragment toggle
            case 118:
                bPerFragment = false;
                bPerVertex = true;
                console.log("Switched to Per-Vertex lighting\n");
                break;
            case 27: // Escape key
                // uninitialize and close
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
        "in vec4 aPosition; \n" +
        "in vec3 aNormal;\n" +
        "uniform mat4 uModelMatrix;\n" +
        "uniform mat4 uViewMatrix;\n" +
        "uniform mat4 uProjectionMatrix;\n" +
        "uniform vec3 uLa;\n" +
        "uniform vec3 uLd;\n" +
        "uniform vec3 uLs;\n" +
        "uniform vec3 uKa;\n" +
        "uniform vec3 uKd;\n" +
        "uniform vec3 uKs;\n" +
        "uniform vec4 uLightPosition;\n" +
        "uniform float uMaterialShininess;\n" +
        "precision highp int;"+
        "uniform int uLKeyIsPressed;\n" +
        "out vec3 out_Phong_ADS_Light;\n" +
        "void main(void)\n" +
        "{\n" +
        "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
        "if(uLKeyIsPressed == 1)\n" +
        "{\n" +
        "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
        "mat3 normalMatrix = mat3(uViewMatrix * uModelMatrix);\n" +
        "vec3 transformedNormal = normalize(normalMatrix * aNormal);\n" +
        "vec3 lightDirection = normalize(vec3(uLightPosition - eyeCoordinates));\n" +
        "vec3 ambientLight = uLa * uKa;\n" +
        "vec3 diffuseLight = uLd * uKd * max(dot(lightDirection, transformedNormal), 0.0);\n" +
        "vec3 reflectionVector = reflect(-lightDirection, transformedNormal);\n" +
        "vec3 viewerVector = normalize(-eyeCoordinates.xyz) ;\n" +
        "vec3 specularLight = uLs * uKs * pow(max(dot(reflectionVector, viewerVector), 0.0), uMaterialShininess);\n" +
        "out_Phong_ADS_Light = ambientLight + diffuseLight + specularLight ;\n" +
        "}\n" +
        "else\n" +
        "{\n" +
        "out_Phong_ADS_Light = vec3(1.0f,1.0f,1.0f);\n" +
        "}\n" +
        "}\n";

        // 2. Create the shader object
        var vertexShaderObject_pv = gl.createShader(gl.VERTEX_SHADER);

        // 3. Give the shader source code to the shader object
        gl.shaderSource(vertexShaderObject_pv, vertexShaderSourceCode);

        // 4. Compile the shader programmatically
	    gl.compileShader(vertexShaderObject_pv);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(vertexShaderObject_pv, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(vertexShaderObject_pv);
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
        "in vec3 out_Phong_ADS_Light;\n" +
        "out vec4 fragColor;\n" +
        "void main(void)\n" +
        "{\n" +
        "fragColor = vec4(out_Phong_ADS_Light, 1.0);\n" +
        "}\n";

        // 2. Create the shader object
        var fragmentShaderObject_pv = gl.createShader(gl.FRAGMENT_SHADER);

        // 3. Give the shader source code to the shader object
        gl.shaderSource(fragmentShaderObject_pv, fragmentShaderSourceCode);

        // 4. Compile the shader programmatically
        gl.compileShader(fragmentShaderObject_pv);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(fragmentShaderObject_pv, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(fragmentShaderObject_pv );
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
        shaderProgramObject_pv = gl.createProgram();
        gl.attachShader(shaderProgramObject_pv, vertexShaderObject_pv);
        gl.attachShader(shaderProgramObject_pv, fragmentShaderObject_pv);

        // Bind shader attribute at a certain index in shader to save index in host program
        gl.bindAttribLocation(shaderProgramObject_pv, MyAttributes.AMC_ATTRIBUTE_POSITION, "aPosition");
        gl.bindAttribLocation(shaderProgramObject_pv, MyAttributes.AMC_ATTRIBUTE_NORMAL, "aNormal");
        gl.linkProgram(shaderProgramObject_pv);

        if(gl.getProgramParameter(shaderProgramObject_pv, gl.LINK_STATUS) == false)
        {
            var error = gl.getProgramInfoLog(shaderProgramObject_pv);
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
        modelMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uModelMatrix");
        viewMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uViewMatrix");
        projectionMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uProjectionMatrix");
        laUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uLa");
        ldUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uLd");
        lsUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uLs");
        kaUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uKa");
        kdUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uKd");
        ksUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uKs");
        lightPositionUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uLightPosition");
        materialShininessUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uMaterialShininess");
        lKeyPressedUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uLKeyIsPressed");

        // ------------------------------------------------------------------------
        // VERTEX SHADER PER FRAGMENT
        // 1. Write shader source code
        var vertexShaderSourceCode_pf =
        "#version 300 es\n"+
        "in vec4 aPosition; \n" +
        "in vec3 aNormal;\n" +
        "uniform mat4 uModelMatrix;\n" +
        "uniform mat4 uViewMatrix;\n" +
        "uniform mat4 uProjectionMatrix;\n" +
        "uniform vec3 uLa;\n" +
        "uniform vec3 uLd;\n" +
        "uniform vec3 uLs;\n" +
        "uniform vec4 uLightPosition;\n" +
        "uniform vec3 uKa;\n" +
        "uniform vec3 uKd;\n" +
        "uniform vec3 uKs;\n" +
        "uniform float uMaterialShininess;\n" +
        "precision highp int;\n" +
        "uniform int uLKeyIsPressed;\n" +
        "out vec3 transformedNormal;\n" +
        "out vec3 lightDirection;\n" +
        "out vec3 viewerVector;\n" +
        "out vec3 ambientLight;\n" +
        "out vec3 diffuseLight;\n" +
        "out vec3 specularLight;\n" +
        "void main(void)\n" +
        "{\n" +
        "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
        "if(uLKeyIsPressed == 1)\n" +
        "{\n" +
        "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
        "mat3 normalMatrix = mat3(uViewMatrix * uModelMatrix);\n" +
        "transformedNormal = normalize(normalMatrix * aNormal);\n" +
        "lightDirection = normalize(vec3(uLightPosition - eyeCoordinates));\n" +
        "viewerVector = normalize(-eyeCoordinates.xyz);\n" +
        "ambientLight = uLa * uKa;\n" +
        "}\n" +
        "}\n";

        // 2. Create the shader object
        var vertexShaderObject_pf = gl.createShader(gl.VERTEX_SHADER);

        // 3. Give the shader source code to the shader object
        gl.shaderSource(vertexShaderObject_pf, vertexShaderSourceCode_pf);

        // 4. Compile the shader programmatically
	    gl.compileShader(vertexShaderObject_pf);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(vertexShaderObject_pf, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(vertexShaderObject_pf);
            if (error.length > 0)
            {
                alert("Error: " + error);
            }
            uninitialize();
        }
        else
        {
            console.log("Vertex shader compilation successful\n");
        }

        // FRAGMENT SHADER PER-FRAGMENT
        // 1. Write shader source code
        var fragmentShaderSourceCode_pf = 
        "#version 300 es\n"+
        "precision highp float;\n" +
        "in vec3 transformedNormal;\n" +
        "in vec3 lightDirection;\n" +
        "in vec3 viewerVector;\n" +
        "in vec3 ambientLight;\n" +
        "uniform vec3 uLd;\n" +
        "uniform vec3 uKd;\n" +
        "uniform vec3 uLs;\n" +
        "uniform vec3 uKs;\n" +
        "uniform float uMaterialShininess;\n" +
        "precision highp int;\n" +
        "uniform int uLKeyIsPressed;\n" +
        "out vec4 fragColor;\n" +
        "void main(void)\n" +
        "{\n" +
        "vec3 phongADS;\n" +
        "if(uLKeyIsPressed == 1)\n" +
        "{\n" +
        "vec3 diffuseLight = uLd * uKd * max(dot(lightDirection, transformedNormal), 0.0);\n" +
        "vec3 reflectionVector = reflect(-lightDirection, transformedNormal);\n" +
        "vec3 specularLight = uLs * uKs * pow(max(dot(reflectionVector, viewerVector), 0.0), uMaterialShininess);\n" +
        "phongADS = ambientLight + diffuseLight + specularLight;\n" +
        "}\n" +
        "else\n" +
        "{\n" +
        "phongADS = vec3(1.0, 1.0, 1.0);\n" +
        "}\n" +
        "fragColor = vec4(phongADS, 1.0);\n" +
        "}\n";

        // 2. Create the shader object
        var fragmentShaderObject_pf = gl.createShader(gl.FRAGMENT_SHADER);

        // 3. Give the shader source code to the shader object
        gl.shaderSource(fragmentShaderObject_pf, fragmentShaderSourceCode_pf);

        // 4. Compile the shader programmatically
        gl.compileShader(fragmentShaderObject_pf);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(fragmentShaderObject_pf, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(fragmentShaderObject_pf);
            if (error.length > 0)
            {
                alert("Error: " + error);
            }
            uninitialize();
        }
        else
        {
            console.log("Fragment shader compilation successful\n");
        }

        // Create, attach, link shader program object
        shaderProgramObject_pf = gl.createProgram();
        gl.attachShader(shaderProgramObject_pf, vertexShaderObject_pf);
        gl.attachShader(shaderProgramObject_pf, fragmentShaderObject_pf);

        // Bind shader attribute at a certain index in shader to save index in host program
        gl.bindAttribLocation(shaderProgramObject_pf, MyAttributes.AMC_ATTRIBUTE_POSITION, "aPosition");
        gl.bindAttribLocation(shaderProgramObject_pf, MyAttributes.AMC_ATTRIBUTE_NORMAL, "aNormal");
        gl.linkProgram(shaderProgramObject_pf);

        if(gl.getProgramParameter(shaderProgramObject_pf, gl.LINK_STATUS) == false)
        {
            var error = gl.getProgramInfoLog(shaderProgramObject_pf);
            if (error.length > 0)
            {
                alert("Error: " + error);
            }
            uninitialize();
        }
        else
        {
            console.log("Shader linking successful\n");
        }

        // Get the required uniform location from the shader
        modelMatrixUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uModelMatrix");
        viewMatrixUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uViewMatrix");
        projectionMatrixUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uProjectionMatrix");
        laUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uLa");
        ldUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uLd");
        lsUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uLs");
        kaUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uKa");
        kdUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uKd");
        ksUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uKs");
        lightPositionUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uLightPosition");
        materialShininessUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uMaterialShininess");
        lKeyPressedUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uLKeyIsPressed");

        // Provide vertex position, color, normal, texCoord etc.
        sphere = new Mesh();
		makeSphere(sphere, 2.0, 30, 30);

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
        // code
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        // Transformations
        var modelMatrix = mat4.create();
        var viewMatrix = mat4.create();
        var translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -6.0]); // translate cube backwards
        var scaleMatrix = mat4.create();
        mat4.scale(scaleMatrix, scaleMatrix, [0.75, 0.75, 0.75]); // scale down to 75%
        var rotationMatrix = mat4.create();
        mat4.rotateY(rotationMatrix, rotationMatrix, Math.PI * angleSphere / 180.0); // rotate about Y axis

        // modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
        mat4.multiply(modelMatrix, translationMatrix, scaleMatrix);
        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix);

        // use shader program object
        if((bPerVertex == true) && (bPerFragment == false))
        {
            gl.useProgram(shaderProgramObject_pv);
            
            // send above matrix to the shader in "uniform"
            gl.uniformMatrix4fv(modelMatrixUniform_pv, false, modelMatrix);
            gl.uniformMatrix4fv(viewMatrixUniform_pv, false, viewMatrix);
            gl.uniformMatrix4fv(projectionMatrixUniform_pv, false, perspectiveProjectionMatrix);

            if (bLight == true)
            {
                gl.uniform3fv(laUniform_pv, lightAmbient);
                gl.uniform3fv(ldUniform_pv, lightDiffuse);
                gl.uniform3fv(lsUniform_pv, lightSpecular);
                gl.uniform4fv(lightPositionUniform_pv, lightPosition);
                gl.uniform3fv(kaUniform_pv, materialAmbient); // material ambient reflectance
                gl.uniform3fv(kdUniform_pv, materialDiffuse); // material diffuse reflectance
                gl.uniform3fv(ksUniform_pv, materialSpecular); // material specular reflectance
                gl.uniform1f(materialShininessUniform_pv, materialShininess); // material shininess
                gl.uniform1i(lKeyPressedUniform_pv, 1);
            }
            else
            {
                gl.uniform1i(lKeyPressedUniform_pv, 0);
            }
        }
        else if((bPerVertex == false) && (bPerFragment == true))
        {
            gl.useProgram(shaderProgramObject_pf);
            
            // send above matrix to the shader in "uniform"
            gl.uniformMatrix4fv(modelMatrixUniform_pf, false, modelMatrix);
            gl.uniformMatrix4fv(viewMatrixUniform_pf, false, viewMatrix);
            gl.uniformMatrix4fv(projectionMatrixUniform_pf, false, perspectiveProjectionMatrix);

            if (bLight == true)
            {
                gl.uniform3fv(laUniform_pf, lightAmbient);
                gl.uniform3fv(ldUniform_pf, lightDiffuse);
                gl.uniform3fv(lsUniform_pf, lightSpecular);
                gl.uniform4fv(lightPositionUniform_pf, lightPosition);
                gl.uniform3fv(kaUniform_pf, materialAmbient); // material ambient reflectance
                gl.uniform3fv(kdUniform_pf, materialDiffuse); // material diffuse reflectance
                gl.uniform3fv(ksUniform_pf, materialSpecular); // material specular reflectance
                gl.uniform1f(materialShininessUniform_pf, materialShininess); // material shininess
                gl.uniform1i(lKeyPressedUniform_pf, 1);
            }
            else
            {
                gl.uniform1i(lKeyPressedUniform_pf, 0);
            }
        }

        sphere.draw();

        // unuse shader program object
        gl.useProgram(null);

        // call update
        update();

        // double buffering
        request_animation_frame(display, canvas);  // here the game loop is maintained via recursion
    }

    function update()
    {
        // Code
        // angleSphere = angleSphere + 0.01;
        // if(angleSphere >= 360.0)
        // {
        //     angleSphere = angleSphere - 360.0;
        // }
    }

    function uninitialize()
    {
        // Code
        if(bFullscreen == true)
        {
            toggleFullscreen();
        } 
        
        if (sphere)
		{
			sphere.deallocate();
			sphere = null;
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


