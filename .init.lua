vim.opt.makeprg = "./build.sh" 

local wk = require('which-key')
wk.register(
  {
    ['<F5>'] = { "<Cmd>make<CR>", "Make" }
  }
  , vim.common.topopts

)
