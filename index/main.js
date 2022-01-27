import Vue from "vue";
// 引入样式
import "vue-easytable/libs/theme-default/index.css";
// 引入组件库
import VueEasytable from "vue-easytable";

Vue.use(VueEasytable);

new Vue({
    el: "#app",
    render: (h) => h(App),
});

