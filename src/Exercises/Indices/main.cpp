#include "app.h"

#include "glad/glad.h"

int main() {

    SimpleShapeApplication app(650, 480, PROJECT_NAME,4,1);


    app.run(1);


    return 0;

}