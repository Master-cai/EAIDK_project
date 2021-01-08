/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * AS IS BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * Copyright (c) 2020, OPEN AI LAB
 * Author: haitao@openailab.com
 */

#include <stdio.h>
#include <assert.h>

#include "sys_port.h"
#include "tengine_ir.h"
#include "tengine_errno.h"
#include "tengine_log.h"
#include "tengine_op.h"
#include "parameter.h"
#include "selu_param.h"

DEFINE_PARM_PARSE_ENTRY(selu_param, alpha, lambda);

static int init_op(struct ir_op* op)
{
    struct selu_param* selu_param = ( struct selu_param* )sys_malloc(sizeof(struct selu_param));

    if (selu_param == NULL)
    {
        set_tengine_errno(ENOMEM);
        return -1;
    }

    /*set the param default value */
    selu_param->alpha = 1.f;
    selu_param->lambda = 1.f;

    op->param_mem = selu_param;
    op->param_size = sizeof(struct selu_param);
    op->same_shape = 1;
    op->infer_shape = NULL;

    return 0;
}

static void release_op(struct ir_op* op)
{
    sys_free(op->param_mem);
}

static int register_selu_op(void* arg)
{
    struct op_method m;

    m.op_version = 1;
    m.init_op = init_op;
    m.release_op = release_op;
    m.access_param_entry = access_param_entry;

    return register_op(OP_SELU, OP_SELU_NAME, &m);
}

static int unregister_selu_op(void* arg)
{
    sys_free(GET_PARAM_PARSE_MAP(selu_param));
    return unregister_op(OP_SELU, 1);
}

AUTO_REGISTER_OP(register_selu_op);
AUTO_UNREGISTER_OP(unregister_selu_op);
