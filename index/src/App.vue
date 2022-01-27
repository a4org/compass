<template>
  <div id="app">
    <el-container style="height: 100%">
      <el-header>
        <Header/>
      </el-header>
      <el-main>
        <CruelBoardTable style="height: 100%"/>
      </el-main>
    </el-container>
  </div>
</template>

<script>
import Header from "@/components/Header";
import CruelBoardTable from './components/CruelBoardTable.vue'

export default {
  name: 'App',
  components: {
    Header,
    CruelBoardTable
  }
}
</script>

<style>
html, body {
  height: 100%;
  margin: 0;
}

#app {
  font-family: "Helvetica Neue",Helvetica,"PingFang SC","Hiragino Sans GB","Microsoft YaHei","微软雅黑",Arial,sans-serif;
  height: 100%;
}

.el-header {
  border-bottom: solid 1px #e6e6e6;
}
</style>
