module.exports = {
  publicPath: './',
  configureWebpack: {
    externals: [{
      './cptable': 'var cptable'
    }]
  }
}