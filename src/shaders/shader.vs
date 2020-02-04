#version 410 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 inormal;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        out vec3 normal;
        out vec3 fragPos;
        void main()
        {
            // Note that we read the multiplication from right to left
            gl_Position = projection * view * model * vec4(position, 1.0f);
            normal = inormal;
            fragPos = vec3( model * vec4(position, 1.0f));
        }
