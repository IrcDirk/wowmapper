#pragma once

#include "../chunk_c.h"

/*! \brief MCVT chunk
 *  \url http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MCVT_sub-chunk */
struct McvtChunk_s : public Chunk_c {
  std::vector<float> heights;

  McvtChunk_s(Chunk_c *parent) : Chunk_c(parent), heights(145) { }

  void GetVertices(Points_t *vertices) const {
    vertices->resize(145);

    for (int i = 0; i < 145; i++) {
      uint32_t row_idx = i / 17;
      uint32_t col_idx = i % 17;

      bool isRow9x9 = col_idx < 9;

      if (isRow9x9) { // if it's a row of the outer 9x9 vector ...
        vertices->at(i).x = col_idx * UNIT * 2 - UNIT;
        vertices->at(i).z = row_idx * UNIT * 2 - UNIT;
      } else {        // if it's a row of the inner 8x8 vector ...
        vertices->at(i).x = (col_idx-9) * UNIT * 2;
        vertices->at(i).z = row_idx * UNIT * 2;
      }
      vertices->at(i).y = heights.at(i);
    }
  }

  void GetIndices(Indices32_t *indices) {
    indices->resize(768);

    uint32_t i = 0;
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        i = y*8*12 + x*12;

        // vertex 0
        indices->at(i + 0) = y * 17 + x;          // top left
        indices->at(i + 1) = y * 17 + x + 1;      // top right
        indices->at(i + 2) = y * 17 + x + 9;      // center

        // vertex 1
        indices->at(i + 3) = indices->at(i + 1);  // top right
        indices->at(i + 4) = y * 17 + x + 18;     // bottom right
        indices->at(i + 5) = indices->at(i + 2);  // center

        // vertex 2
        indices->at(i + 6) = indices->at(i + 4);  // bottom right
        indices->at(i + 7) = y * 17 + x + 17;     // bottom left
        indices->at(i + 8) = indices->at(i + 2);  // center

        // vertex 3
        indices->at(i + 9) = indices->at(i + 7);  // bottom left
        indices->at(i + 10) = indices->at(i + 0); // top left
        indices->at(i + 11) = indices->at(i + 2); // center
      }
    }
  }

 protected:
  virtual void Initialize() { CopyDataBlock(buffer_, &heights); }
};