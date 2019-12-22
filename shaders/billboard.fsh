varying vec4 uv;
uniform sampler2D texture;
uniform vec3 color;
uniform float elapsed;

void main() {
    gl_FragColor = texture2D(texture, uv.st) * vec4(color, elapsed);
}
