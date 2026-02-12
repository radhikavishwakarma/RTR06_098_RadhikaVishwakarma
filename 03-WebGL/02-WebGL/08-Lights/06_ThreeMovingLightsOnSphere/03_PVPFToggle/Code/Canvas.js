var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;
var request_animation_frame = window.requestAnimationFrame ||
                                window.mozRequestAnimationFrame ||
                                window.oRequestAnimationFrame ||
                                window.webkitRequestAnimationFrame ||
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

var lightAngle0 = 0.0; // for first light
var lightAngle1 = 0.0; // for second light
var lightAngle2 = 0.0; // for third light

var modelMatrixUniform_pv;
var viewMatrixUniform_pv;
var projectionMatrixUniform_pv;
var modelMatrixUniform_pf;
var viewMatrixUniform_pf;
var projectionMatrixUniform_pf;

var angleSphere = 0.0;

// Parameters to be passed as uniform to shaders for light calculations
var laUniform_pv = new Array(3);
var ldUniform_pv = new Array(3);
var lsUniform_pv = new Array(3);
var lightPositionUniform_pv = new Array(3);
var kaUniform_pv = 0;
var kdUniform_pv = 0;
var ksUniform_pv = 0;
var materialShininessUniform_pv = 0;

var laUniform_pf = new Array(3);
var ldUniform_pf = new Array(3);
var lsUniform_pf = new Array(3);
var lightPositionUniform_pf = new Array(3);
var kaUniform_pf = 0;
var kdUniform_pf = 0;
var ksUniform_pf = 0;
var materialShininessUniform_pf = 0;

var lKeyPressedUniform_pv = 0;
var lKeyPressedUniform_pf = 0;
var perspectiveProjectionMatrix;

var bAnimation = false;
var bLight = false;
var bPerVertex = true;
var bPerFragment = false;

/// Lights related variable
class Light {
    constructor() {
        this.ambient = [0.0, 0.0, 0.0, 1.0];  // Equivalent to vec4
        this.diffuse = [0.0, 0.0, 0.0, 1.0];
        this.specular = [0.0, 0.0, 0.0, 1.0];
        this.position = [0.0, 0.0, 0.0, 1.0];
    }
}

const light = [
    new Light(), // light[0]
    new Light(),  // light[1]
    new Light()  // light[2]
];

var materialAmbient = [0.0, 0.0, 0.0];
var materialDiffuse = [0.5, 0.2, 0.7];
var materialSpecular = [0.7, 0.7, 0.7];
var materialShininess = 50.0;

function main()
{
    // get canvas
    canvas = document.getElementById("AMC")
    if (canvas == null)
    {
        console.log("Canvas element cannot be obtained.\n");   
    }
    else
    {
        console.log("Canvas element succesfully obtained.\n");   
    }

    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;

    // register our callback functions as event listeners
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false);

    // initialize
    initialize();

    // resize
    resize();

    // display
    display();

    function keyDown(event)
    {
        // code
        // alert("Key is pressed!!\n");
        switch (event.keyCode)
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
        // alert("Mouse is clicked!!\n");
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
            if(canvas.requestFullscreen)
            {
                canvas.requestFullscreen();
            }
            else if(canvas.mozRequestFullScreen)
            {
                canvas.mozRequestFullScreen();
            }
            else if(canvas.webkitRequestFullScreen)
            {
                canvas.webkitRequestFullScreen();
            }
            else if(canvas.msRequestFullScreen)
            {
                canvas.msRequestFullScreen();
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

    // stub functions
    function initialize()
    {
        // code
        // get 2D context from canvas
        gl = canvas.getContext("webgl2");
        if (gl == null)
        {
            console.log("WebGL2 Context element cannot be obtained.\n");   
        }
        else
        {
            console.log("WebGL2 Context element succesfully obtained.\n");   
        }

        // set viewport width and viewport height
        gl.viewportWidth = canvas.width;
        gl.viewportHeight = canvas.height;

        // VERTEX SHADER PER-VERTEX
        // 1. Write shader source code
        var vertexShaderSourceCode_pv = 
        "#version 300 es\n"+
        "in vec4 aPosition; \n" +
        "in vec3 aNormal;\n" +
        "uniform mat4 uModelMatrix;\n" +
        "uniform mat4 uViewMatrix;\n" +
        "uniform mat4 uProjectionMatrix;\n" +
        "uniform vec3 uLa[3];\n" +
        "uniform vec3 uLd[3];\n" +
        "uniform vec3 uLs[3];\n" +
        "uniform vec4 uLightPosition[3];\n" +
        "uniform vec3 uKa;\n" +
        "uniform vec3 uKd;\n" +
        "uniform vec3 uKs;\n" +
        "uniform float uMaterialShininess;\n" +
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
                "vec3 viewerVector = normalize(-eyeCoordinates.xyz);\n" +
                "vec3 lightDirection[3];\n" +
                "vec3 ambientLight[3];\n" +
                "vec3 diffuseLight[3];\n" +
                "vec3 reflectionVector[3];\n" +
                "vec3 specularLight[3];\n" +
                "out_Phong_ADS_Light = vec3(0.0f, 0.0f, 0.0f);\n" +
                "for(int i = 0; i<3; i++)\n" +
                "{\n" +
                    "lightDirection[i] = normalize(vec3(uLightPosition[i] - eyeCoordinates));\n" +
                    "ambientLight[i] = uLa[i] * uKa;\n" +
                    "diffuseLight[i] = uLd[i] * uKd * max(dot(lightDirection[i],transformedNormal),0.0);\n" +
                    "reflectionVector[i] = reflect(-lightDirection[i], transformedNormal);\n" +
                    "specularLight[i] = uLs[i] * uKs * pow(max(dot(reflectionVector[i], viewerVector), 0.0), uMaterialShininess);\n" +
                    "out_Phong_ADS_Light = out_Phong_ADS_Light + ambientLight[i] + diffuseLight[i] + specularLight[i];\n" +
                "}\n" +
            "}\n" +
            "else\n" +
            "{\n" +
                "out_Phong_ADS_Light = vec3(1.0f, 1.0f, 1.0f);\n" +
            "}\n" +
        "}\n";

        // 2. Create the shader object
        var vertexShaderObject_pv = gl.createShader(gl.VERTEX_SHADER);

        // 3. Give the shader source code to the shader object
        gl.shaderSource(vertexShaderObject_pv, vertexShaderSourceCode_pv);

        // 4. Compile the shader programmatically
	    gl.compileShader(vertexShaderObject_pv);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(vertexShaderObject_pv, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(vertexShaderObject_pv);
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

        // FRAGMENT SHADER PER-VERTEX
        // 1. Write shader source code
        var fragmentShaderSourceCode_pv = 
        "#version 300 es\n"+
        "precision highp float;\n" +
        "in vec3 out_Phong_ADS_Light;\n" +
        "out vec4 fragColor;\n" +
        "void main(void)\n" +
        "{\n" +
        "fragColor = vec4(out_Phong_ADS_Light, 1.0);\n" +
        "}\n";

        // 2. Create the shader object
        var fragmentShaderObject_pv = gl.createShader(gl.FRAGMENT_SHADER);

        // 3. Give the shader source code to the shader object
        gl.shaderSource(fragmentShaderObject_pv, fragmentShaderSourceCode_pv);

        // 4. Compile the shader programmatically
        gl.compileShader(fragmentShaderObject_pv);

        // 5. Do shader compilation error checking
        if(gl.getShaderParameter(fragmentShaderObject_pv, gl.COMPILE_STATUS) == false)
        {
            var error = gl.getShaderInfoLog(fragmentShaderObject_pv);
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
        modelMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uModelMatrix");
        viewMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uViewMatrix");
        projectionMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uProjectionMatrix");
        laUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "uLa[0]");
        ldUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "uLd[0]");
        lsUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "uLs[0]");
        lightPositionUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "uLightPosition[0]");
        laUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "uLa[1]");
        ldUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "uLd[1]");
        lsUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "uLs[1]");
        lightPositionUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "uLightPosition[1]");
        laUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "uLa[2]");
        ldUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "uLd[2]");
        lsUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "uLs[2]");
        lightPositionUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "uLightPosition[2]");
        kaUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uKa");
        kdUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uKd");
        ksUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uKs");
        materialShininessUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uMaterialShininess");
        lKeyPressedUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "uLKeyIsPressed");
        // -----------------------------------------------------------
        // VERTEX SHADER PER-FRAGMENT
        // 1. Write shader source code
        var vertexShaderSourceCode_pf = 
        "#version 300 es\n"+
        "in vec4 aPosition;\n" +
        "in vec3 aNormal;\n" +
        "uniform mat4 uModelMatrix;\n" +
        "uniform mat4 uViewMatrix;\n" +
        "uniform mat4 uProjectionMatrix;\n" +
        "uniform vec4 uLightPosition[3];\n" +
        "precision highp int;\n" +
        "uniform int uLKeyIsPressed;\n" +
        "out vec3 out_transformedNormal;\n" +
        "out vec3 out_lightDirection[3];\n" +
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
                "for(int i = 0; i<3; i++)\n" +
                "{\n" +
                    "out_lightDirection[i] = vec3(uLightPosition[i] - eyeCoordinates);\n" +
                "}\n" +
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
        "in vec3 out_transformedNormal;\n" +
        "in vec3 out_lightDirection[3];\n" +
        "in vec3 out_viewerVector;\n" +
        "uniform vec3 uLa[3];\n" +
        "uniform vec3 uLd[3];\n" +
        "uniform vec3 uLs[3];\n" +
        "uniform vec3 uKa;\n" +
        "uniform vec3 uKd;\n" +
        "uniform vec3 uKs;\n" +
        "uniform float uMaterialShininess;\n" +
        "precision highp int;\n" +
        "uniform int uLKeyIsPressed;\n" +
        "out vec4 fragColor;\n" +
        "void main(void)\n" +
        "{\n" +
            "vec3 phong_ADS_Light;\n" +
            "vec3 normalizedLightDirection[3];\n" +
            "vec3 ambientLight[3];\n" +
            "vec3 diffuseLight[3];\n" +
            "vec3 reflectionVector[3];\n" +
            "vec3 specularLight[3];\n" +
            "if(uLKeyIsPressed == 1)\n" +
            "{\n" +
                "vec3 normalizedTransformedNormal = normalize(out_transformedNormal);\n" +
                "vec3 normalizedViewerVector = normalize(out_viewerVector);\n" +
                "for(int i = 0; i<3; i++)\n" +
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
        laUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "uLa[0]");
        ldUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "uLd[0]");
        lsUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "uLs[0]");
        lightPositionUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "uLightPosition[0]");
        laUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "uLa[1]");
        ldUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "uLd[1]");
        lsUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "uLs[1]");
        lightPositionUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "uLightPosition[1]");
        laUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "uLa[2]");
        ldUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "uLd[2]");
        lsUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "uLs[2]");
        lightPositionUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "uLightPosition[2]");
        kaUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uKa");
        kdUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uKd");
        ksUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uKs");
        materialShininessUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uMaterialShininess");
        lKeyPressedUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "uLKeyIsPressed");

        // Initializations of two lights
        light[0].ambient = [0.0, 0.0, 0.0, 1.0];
        light[0].diffuse = [1.0, 0.0, 0.0, 1.0];
        light[0].specular = [1.0, 0.0, 0.0, 1.0];
        // light[0].position = [-2.0, 0.0, 0.0, 1.0];
        light[1].ambient = [0.0, 0.0, 0.0, 1.0];
        light[1].diffuse = [0.0, 1.0, 0.0, 1.0];
        light[1].specular = [0.0, 1.0, 0.0, 1.0];
        // light[1].position = [2.0, 0.0, 0.0, 1.0];
        light[2].ambient = [0.0, 0.0, 0.0, 1.0];
        light[2].diffuse = [0.0, 0.0, 1.0, 1.0];
        light[2].specular = [0.0, 0.0, 1.0, 1.0];
        // light[2].position = [0.0, 2.0, 0.0, 1.0];

        // Provide vertex position, color, normal, texCoord etc.
        sphere = new Mesh();
		makeSphere(sphere, 2.0, 30, 30);

        // Depth related code
        gl.clearDepth(1.0);
        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc(gl.LEQUAL);
        
        // Tell OpenGL to choose the color to clear the screen
        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        
        perspectiveProjectionMatrix = mat4.create(); // this is similar to glLoadIdentity() in resize
    }

    function resize()
    {
        // code 
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

        var radius = 3.0;

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
                light[0].position = [0.0, radius * Math.sin(Math.PI/180.0 * lightAngle0), radius * Math.cos(Math.PI/180.0 * lightAngle0), 1.0];
                light[1].position = [radius * Math.sin(Math.PI/180.0 * lightAngle1), 0.0, radius * Math.cos(Math.PI/180.0 * lightAngle1), 1.0];
                light[2].position = [radius * Math.cos(Math.PI/180.0 * lightAngle2), radius * Math.sin(Math.PI/180.0 * lightAngle2), 0.0, 1.0];
                
                gl.uniform3fv(laUniform_pv[0], light[0].ambient.slice(0,3));
                gl.uniform3fv(ldUniform_pv[0], light[0].diffuse.slice(0,3));
                gl.uniform3fv(lsUniform_pv[0], light[0].specular.slice(0,3));
                gl.uniform4fv(lightPositionUniform_pv[0], light[0].position);
                gl.uniform3fv(laUniform_pv[1], light[1].ambient.slice(0,3));
                gl.uniform3fv(ldUniform_pv[1], light[1].diffuse.slice(0,3));
                gl.uniform3fv(lsUniform_pv[1], light[1].specular.slice(0,3));
                gl.uniform4fv(lightPositionUniform_pv[1], light[1].position);
                gl.uniform3fv(laUniform_pv[2], light[2].ambient.slice(0,3));
                gl.uniform3fv(ldUniform_pv[2], light[2].diffuse.slice(0,3));
                gl.uniform3fv(lsUniform_pv[2], light[2].specular.slice(0,3));
                gl.uniform4fv(lightPositionUniform_pv[2], light[2].position);
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
                light[0].position = [0.0, radius * Math.sin(Math.PI/180.0 * lightAngle0), radius * Math.cos(Math.PI/180.0 * lightAngle0), 1.0];
                light[1].position = [radius * Math.sin(Math.PI/180.0 * lightAngle1), 0.0, radius * Math.cos(Math.PI/180.0 * lightAngle1), 1.0];
                light[2].position = [radius * Math.cos(Math.PI/180.0 * lightAngle2), radius * Math.sin(Math.PI/180.0 * lightAngle2), 0.0, 1.0];
                
                gl.uniform3fv(laUniform_pf[0], light[0].ambient.slice(0,3));
                gl.uniform3fv(ldUniform_pf[0], light[0].diffuse.slice(0,3));
                gl.uniform3fv(lsUniform_pf[0], light[0].specular.slice(0,3));
                gl.uniform4fv(lightPositionUniform_pf[0], light[0].position);
                gl.uniform3fv(laUniform_pf[1], light[1].ambient.slice(0,3));
                gl.uniform3fv(ldUniform_pf[1], light[1].diffuse.slice(0,3));
                gl.uniform3fv(lsUniform_pf[1], light[1].specular.slice(0,3));
                gl.uniform4fv(lightPositionUniform_pf[1], light[1].position);
                gl.uniform3fv(laUniform_pf[2], light[2].ambient.slice(0,3));
                gl.uniform3fv(ldUniform_pf[2], light[2].diffuse.slice(0,3));
                gl.uniform3fv(lsUniform_pf[2], light[2].specular.slice(0,3));
                gl.uniform4fv(lightPositionUniform_pf[2], light[2].position);
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

        // Draw sphere
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
        // code
        // rotation
        lightAngle0 += 0.2;
        if(lightAngle0 >= 360.0)
        {
            lightAngle0 = lightAngle0 - 360.;
        }

        // update green light position
        lightAngle1 += 0.2;
        if(lightAngle1 >= 360.0)
        {
            lightAngle1 = lightAngle1 - 360.;
        }

        // update blue light position
        lightAngle2 += 0.2;
        if(lightAngle2 >= 360.0)
        {
            lightAngle2 = lightAngle2 - 360.0;
        }
    }

    function uninitialize()
    {
        // code
        if (bFullscreen == true) 
        {
            toggleFullscreen();
        }

        if (sphere)
		{
			sphere.deallocate();
			sphere = null;
		}

        if (shaderProgramObject_pv)
        {
            gl.useProgram(shaderProgramObject_pv);
            var shaderObjects = gl.getAttachedShaders(shaderProgramObject_pv);
            for (let i = 0; i < shaderObjects.length; i++)
            {
                gl.detachShader(shaderProgramObject_pv, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null; 
            }
            gl.useProgram(null);
            gl.deleteProgram(shaderProgramObject_pv);
            shaderProgramObject_pv = null;
        }
    }
}