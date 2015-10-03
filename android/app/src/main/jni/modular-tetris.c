#include <string.h>
#include <jni.h>

#include "board.h"
#include "matrix_file.h"

static struct Board *game_board;

void from_cboard_to_jboard(JNIEnv *env, struct Board *board_struct, jobject board_object)
{
    jclass cls = (*env)->GetObjectClass(env, board_object);

    // TODO: Why to use DeleteLocalRef
    // Setting isGameOver Field
    jfieldID game_over_field = (*env)->GetFieldID(env, cls, "isGameOver", "Z");
    (*env)->SetBooleanField(env, board_object, game_over_field, (jboolean) board_struct->is_game_over);

    // TODO; Put this into a function so current_block and board_values can use the same function

    convert_color_matrix(env, board_object, cls, "boardValues", board_struct->visited, board_struct->height, board_struct->width);
//    convert_color_matrix(env, board_object, cls, "currentBlock", board_struct->visited, board_struct->height, board_struct->width);
}

void convert_color_matrix(JNIEnv *env, jobject board_object, jclass cls, char *field_name, enum Color **color_matrix, int height, int width) {

    jfieldID board_value_field = (*env)->GetFieldID(env, cls, field_name, "[[Lbr/com/gjhenrique/modulartetris/Board$Color;");
    jobject lines = (*env)->GetObjectField(env, board_object, board_value_field);

    // TODO: If it is null, instantiate the new vector
    for (int i = 0; i < height; i++) {
        jobject column = (*env)->GetObjectArrayElement(env, lines, i);
        for (int j = 0; j < width; j++) {

            jclass enum_class = (*env)->FindClass(env, "br/com/gjhenrique/modulartetris/Board$Color");
            jfieldID fieldEnum = (*env)->GetStaticFieldID(env, enum_class, color_strings[color_matrix[i][j]],
                                                          "Lbr/com/gjhenrique/modulartetris/Board$Color;");
            jobject status_one = (*env)->GetStaticObjectField(env, enum_class, fieldEnum);
            (*env)->SetObjectArrayElement(env, column, j, status_one);
            (*env)->DeleteLocalRef(env, status_one);
        }
    }

}


JNIEXPORT jstring JNICALL
Java_br_com_gjhenrique_modulartetris_MainActivity_createBoard(JNIEnv *env, jobject instance,
                                                              jstring boardStr_, jint width, jint height) {

    const char *board_str = (*env)->GetStringUTFChars(env, boardStr_, NULL);

    struct BlockList *blockList = read_from_string(board_str);
    game_board = create_board((int) width, (int) height, blockList);

    // Calling the Board constructor
    jclass cls = (*env)->FindClass(env, "br/com/gjhenrique/modulartetris/Board");
    jobject constructorMethod = (*env)->GetMethodID(env, cls, "<init>", "(II)V");
    jobject newObj = (*env)->NewObject(env, cls, constructorMethod, width, height);

    from_cboard_to_jboard(env, game_board, newObj);

    return newObj;
}

JNIEXPORT void JNICALL
Java_br_com_gjhenrique_modulartetris_Board_nextMove(JNIEnv *env, jobject instance) {
    next_move(game_board);
    from_cboard_to_jboard(env, game_board, instance);
}

JNIEXPORT void JNICALL
Java_br_com_gjhenrique_modulartetris_Board_moveToRight(JNIEnv *env, jobject instance)
{
    move_to_right(game_board)
    from_cboard_to_jboard(env, game_board, instance);
}

JNIEXPORT void JNICALL
Java_br_com_gjhenrique_modulartetris_Board_moveToLeft(JNIEnv *env, jobject instance)
{
    move_to_left(game_board);
    from_cboard_to_jboard(env, game_board, instance);
}

JNIEXPORT void JNICALL
Java_br_com_gjhenrique_modulartetris_Board_rotateClockwise(JNIEnv *env, jobject instance)
{
    rotate_clockwise(game_board);
    from_cboard_to_jboard(env, game_board, instance);
}

JNIEXPORT void JNICALL
Java_br_com_gjhenrique_modulartetris_Board_moveToBottom(JNIEnv *env, jobject instance) {
    move_to_bottom(game_board);
    from_cboard_to_jboard(env, game_board, instance);
}