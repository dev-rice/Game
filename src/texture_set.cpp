#include "texture_set.h"

TextureSet::TextureSet(){
    load(0, 0, 0, 0);
}

TextureSet::TextureSet(GLuint diff, GLuint spec, GLuint norm, GLuint emit){
    load(diff, spec, norm, emit);

    cycle_length = 0;

   diff_frame_count = 0;
   diff_current_index = 0;

   spec_frame_count = 0;
   spec_current_index = 0;

   norm_frame_count = 0;
   norm_current_index = 0;

   emit_frame_count = 0;
   emit_current_index = 0;
}

void TextureSet::makeCyclic(int l){
    cycle_length = l;
}

void TextureSet::load(GLuint diff, GLuint spec, GLuint norm, GLuint emit){
    GLuint pink = TextureLoader::loadPink();
    GLuint alpha = TextureLoader::loadAlpha();

    if (diff == 0){
        if (emit == 0){
            this->diffuse_set.push_back(pink);
            this->specular_set.push_back(pink);
            this->normal_set.push_back(pink);
            this->emissive_set.push_back(pink);
        } else {
            this->diffuse_set.push_back(alpha);
            this->specular_set.push_back(alpha);
            this->normal_set.push_back(alpha);
            this->emissive_set.push_back(emit);
        }

    }
    else {
        this->diffuse_set.push_back(diff);
        
        if (spec == 0){
            this->specular_set.push_back(alpha);
        } else {
            this->specular_set.push_back(spec);
        }

        if (norm == 0){
            this->normal_set.push_back(alpha);
        } else {
            this->normal_set.push_back(norm);
        }

        if (emit == 0){
            this->emissive_set.push_back(alpha);
        } else {
            this->emissive_set.push_back(emit);
        }
        
    }
}

GLuint TextureSet::getDiffuse(){
    if(cycle_length == 0){
        return diffuse_set[0];
    }

    if(diff_frame_count < cycle_length){
        diff_frame_count++;
    } else {
        diff_current_index++;
        diff_frame_count = 0;
    }

    return diffuse_set[diff_current_index % diffuse_set.size()]; 
}

GLuint TextureSet::getSpecular(){
    if(cycle_length == 0){
        return specular_set[0];
    }

    if(spec_frame_count < cycle_length){
        spec_frame_count++;
    } else {
        spec_current_index++;
        spec_frame_count = 0;
    }

    return specular_set[spec_current_index % diffuse_set.size()]; 
}

GLuint TextureSet::getNormal(){
    if(cycle_length == 0){
        return normal_set[0];
    }

    if(norm_frame_count < cycle_length){
        norm_frame_count++;
    } else {
        norm_current_index++;
        norm_frame_count = 0;
    }

    return normal_set[norm_current_index % diffuse_set.size()]; 
}

GLuint TextureSet::getEmissive(){
    if(cycle_length == 0){
        return emissive_set[0];
    }

    if(emit_frame_count < cycle_length){
        emit_frame_count++;
    } else {
        emit_current_index++;
        emit_frame_count = 0;
    }

    return emissive_set[emit_current_index % diffuse_set.size()]; 
}