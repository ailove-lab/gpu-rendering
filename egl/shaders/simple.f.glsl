#version 300 es

in lowp vec3 color;
out lowp vec4 fragmentColor;

void main() {
    fragmentColor = vec4(color, 1.0);
}
