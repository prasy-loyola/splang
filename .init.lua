vim.opt.makeprg = "./build.sh && ./main"

local wk = require('which-key')
wk.register(
  {
    ['<F5>'] = { "<Cmd>make<CR>", "Make" }
  }
  , vim.common.topopts

)
