#include "graphics.hpp"
#include "data.hpp"

const std::string graphics::GENERIC_VSHADER_PATH("shaders/vertex.glsl");
const std::string graphics::GENERIC_FSHADER_PATH("shaders/frag.glsl");
const std::string graphics::GENEREIC_SHADER_MAT("mvp");

static constexpr std::size_t INDEX_VBO = 0;
static constexpr std::size_t VERTEX_VBO = 1;
static constexpr std::size_t POS_ATTRIB = 0;
static constexpr std::size_t COLOR_ATTRIB = 1;
static constexpr std::size_t EXTRA_ATTRIB = 2;


static constexpr std::size_t FUEL_VERT_OFFSET = 0;
static constexpr std::size_t SHIP_VERT_OFFSET = FUEL_VERT_OFFSET + data::fuel::verticesCount;
static constexpr std::size_t TERRAIN_VERT_OFFSET = SHIP_VERT_OFFSET + data::ship::verticesCount;


static constexpr std::size_t FUEL_INDICIES_OFFSET = 0;
static constexpr std::size_t FUEL_OUTER_INDICIES_OFFSET = 12;
static constexpr std::size_t FUEL_INNER_INDICIES_OFFSET = 0;

static constexpr std::size_t SHIP_INDICIES_OFFSET = FUEL_INDICIES_OFFSET + data::fuel::indiciesCount;

static constexpr std::size_t TERRAIN_INDICIES_OFFSET = SHIP_INDICIES_OFFSET + data::ship::indiciesCount;

graphics::Graphics::Graphics() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // create buffers
    glGenBuffers(2, vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[VERTEX_VBO]);
    // initialize storage
    glBufferData(GL_ARRAY_BUFFER, data::totalVertices * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
    // copy fuel data
    glBufferSubData(GL_ARRAY_BUFFER, FUEL_VERT_OFFSET * sizeof(Vertex),
                     data::fuel::verticesCount * sizeof(Vertex), &data::fuel::vertices[0] );
    // copy ship data
    glBufferSubData(GL_ARRAY_BUFFER, SHIP_VERT_OFFSET * sizeof(Vertex),
                    sizeof(Vertex) * data::ship::verticesCount, &data::ship::vertices[0]);
    // copy terrain data
    glBufferSubData(GL_ARRAY_BUFFER, TERRAIN_VERT_OFFSET * sizeof(Vertex),
                    sizeof(Vertex) * data::terrain::verticesCount ,&data::terrain::vertices[0]);
    // setup attributes
    glVertexAttribPointer(POS_ATTRIB, Vertex::posSize(), Vertex::type(), GL_FALSE, Vertex::posStride(),
                            Vertex::posOffset());
    glEnableVertexAttribArray(POS_ATTRIB);
    glVertexAttribPointer(COLOR_ATTRIB, Vertex::colorSize(), Vertex::type(), GL_FALSE, Vertex::colorStride(), 
                Vertex::colorOffset());
    glEnableVertexAttribArray(COLOR_ATTRIB);
    glVertexAttribPointer(EXTRA_ATTRIB, Vertex::extraSize(), Vertex::type(), GL_FALSE, Vertex::extraStride(),
                Vertex::extraOffset());
    glEnableVertexAttribArray(EXTRA_ATTRIB);
    // setup indicies
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[INDEX_VBO]);
    std::vector<GLushort> indicies(data::totalIndicies);
    for(std::size_t i = 0; i < data::fuel::indiciesCount; i++) {
        indicies[i + FUEL_INDICIES_OFFSET] = data::fuel::indicies[i] + FUEL_VERT_OFFSET;
    }
    for(std::size_t i = 0; i < data::ship::indiciesCount; i++) {
        indicies[i + SHIP_INDICIES_OFFSET] = data::ship::indicies[i] + SHIP_VERT_OFFSET;
    }

    for(std::size_t i = 0 ; i < data::terrain::indiciesCount; i++) {
        indicies[i + TERRAIN_INDICIES_OFFSET] = data::terrain::indicies[i] + TERRAIN_VERT_OFFSET;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data::totalIndicies * sizeof(GLushort), &indicies[0], GL_STATIC_DRAW);
}

void graphics::Graphics::drawFuel() {
    glDrawElements(GL_TRIANGLES, data::fuel::innerIndiciesCount, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * FUEL_INNER_INDICIES_OFFSET));
    glDrawElements(GL_LINE_LOOP, data::fuel::outerIndiciesCount, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * FUEL_OUTER_INDICIES_OFFSET));
}

void graphics::Graphics::drawShip() {
    glDrawElements(GL_TRIANGLES, data::ship::indiciesCount, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * SHIP_INDICIES_OFFSET));
}

void graphics::Graphics::drawTerrain() {
    glDrawElements(GL_LINES, data::terrain::indiciesCount, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * TERRAIN_INDICIES_OFFSET));
}

void graphics::Graphics::updateShipModel() {
    glBufferSubData(GL_ARRAY_BUFFER, SHIP_VERT_OFFSET * sizeof(Vertex),
                    sizeof(Vertex) * data::ship::verticesCount, &data::ship::vertices[0]);

}

void graphics::Graphics::updateTerrainModel() {
    glBufferSubData(GL_ARRAY_BUFFER, TERRAIN_VERT_OFFSET * sizeof(Vertex),
                    sizeof(Vertex) * data::terrain::verticesCount ,&data::terrain::vertices[0]);
}

void graphics::Graphics::updateFuelModel() {
    glBufferSubData(GL_ARRAY_BUFFER, FUEL_VERT_OFFSET * sizeof(Vertex),
                     data::fuel::verticesCount * sizeof(Vertex), &data::fuel::vertices[0] );
}

graphics::Graphics::~Graphics() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}