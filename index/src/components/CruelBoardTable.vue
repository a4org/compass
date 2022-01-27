<template>
  <div>
    <div id="boardCheckboxGroup" style="height: 2rem; white-space: nowrap">
      <el-checkbox label="残酷排名" size="mini" v-model="cruelRankingColVisible"></el-checkbox>
      <!--<el-checkbox label="Company" size="mini" v-model="companyVisible"></el-checkbox>-->
      <el-checkbox label="Days" size="mini" v-model="daysColVisible"></el-checkbox>
      <el-checkbox label="工号" size="mini" v-model="workNumColVisible"></el-checkbox>
      <el-checkbox label="LC Rating" size="mini" v-model="ratingColVisible"></el-checkbox>
      <el-checkbox size="mini" :value="true">
        显示最近
        <el-select v-model="contestsShowingNumRaw" allow-create filterable default-first-option size="mini" style="width: 60px">
          <el-option v-for="item in contestsNumOptions" :key="item" :value="item"></el-option>
        </el-select>
        场周赛
      </el-checkbox>
    </div>
    <el-table
        id="boardTable"
        :data="qunyouData"
        style="width: 100%"
        :row-style="{height: '30px'}"
        :cell-style="{padding: '0'}"
        :header-row-style="{height: '40px'}"
        :header-cell-style="{padding: '0px'}"
        stripe
        height="calc(100% - 1.8rem)"
        :default-sort = "{prop: 'cruelScore', order: 'ascending'}">
      <el-table-column
          type="index">
        <template v-slot="scope">
          <el-tooltip
              :disabled="colorTipDisabled"
              effect="light"
              placement="left"
              transition="el-fade-in"
              :enterable="false"
              :open-delay="0">
            <div slot="content">红色 红包A组<br/>蓝色 红包B组</div>
            <span
                :style="`color: #${scope.row.redPacketGroup === 1 ? 'CC3333' : '3366CC'}`">
              {{ `${scope.$index + 1}` }}
            </span>
          </el-tooltip>
        </template>
      </el-table-column>
      <el-table-column
          align="center"
          prop="cruelRanking"
          label="残酷排名"
          width="48"
          v-if="cruelRankingColVisible">
        <template slot="header">
          <div style="font-size: 13px; line-height: 1.3">残酷排名</div>
        </template>
      </el-table-column>
      <el-table-column
          label="LC ID"
          width="120"
          align="center">
        <template v-slot="scope">
          <a :href="scope.row.lcLink">{{ `${scope.row.lcId}` }}</a>
        </template>
      </el-table-column>
      <!--<el-table-column-->
      <!--    width="40"-->
      <!--    v-if="companyVisible"-->
      <!--    align="center">-->
      <!--  <template v-slot="scope">-->
      <!--    <el-image-->
      <!--        v-if="scope.row.company.length > 0"-->
      <!--        style="display: flex"-->
      <!--        :src="`https://board.cruelcoding.com/Img/${scope.row.company}.png`" -->
      <!--        :alt="scope.row.company"-->
      <!--        fit="contain">-->
      <!--    </el-image>-->
      <!--  </template>-->
      <!--</el-table-column>-->
      <el-table-column
          align="center"
          prop="days"
          label="Days"
          width="80"
          sortable
          v-if="daysColVisible">
      </el-table-column>
      <el-table-column
          align="center"
          prop="workNum"
          label="工号"
          width="80"
          sortable
          v-if="workNumColVisible">
      </el-table-column>
      <el-table-column
          align="center"
          prop="lcRating"
          label="Rating"
          width="100"
          sortable
          v-if="ratingColVisible">
        <template v-slot="scope">
          <span :style="`color: #${scope.row.lcRating >= 2100 ? 'A30000' : '0426A4'}`">
            {{ scope.row.lcRating }}
          </span>
        </template>
      </el-table-column>
      <el-table-column
          align="center"
          prop="cruelScore"
          label="滚动积分"
          width="100"
          sortable
          :sort-by="'cruelRanking'"
          :sort-orders="['ascending', 'descending']">
      </el-table-column>
      <el-table-column
          align="center"
          v-for="ci in contestsShowingNum < contests.length ? contestsShowingNum : contests.length" :key="ci"
          :prop="`contest${contests[ci-1].contestIndex}Ranking`"
          :label="`${contests[ci-1].contestIndex} (${contests[ci-1].participantNum})`"
          sortable
          width="100">
        <template slot="header">
          <span style="display: inline-block">
            <div style="font-size: 13px; line-height: 1.3">{{ contests[ci-1].contestIndex }}场</div>
            <div style="font-size: 11px; line-height: 1.1">{{ contests[ci-1].participantNum }}人</div>
          </span>
        </template>
        <template v-slot="scope">
          <el-tooltip
              :disabled="colorTipDisabled"
              effect="light"
              placement="left"
              transition="el-fade-in"
              :enterable="false"
              :open-delay="1000">
            <div slot="content">深绿色 4 题<br/>鲜绿色 3 题<br/>亮黄色 2 题<br/>杏仁白 1 题</div>
            <div :style="`background: #${scope.row.contestRankings[ci-1].rankingClr}; color: black`" @click="colorTipDisabled = !colorTipDisabled">
              {{
                scope.row.contestRankings[ci-1].attendance === -2 ?
                    "未入群" :
                    scope.row.contestRankings[ci-1].attendance === -1 ?
                        "缺赛" :
                        `${scope.row.contestRankings[ci-1].ranking} | ${scope.row.contestRankings[ci-1].score}`
              }}
            </div>
          </el-tooltip>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
import XLSX from 'xlsx-style';

const CONTESTS_SHOWING_NUM_MIN = 3;
const CONTESTS_SHOWING_NUM_SPAN = 8;

export default {
  name: 'CruelBoardTable',
  data() {
    return {
      // companyVisible: false,
      daysColVisible: false,
      workNumColVisible: false,
      ratingColVisible: true,
      cruelRankingColVisible: false,
      contestsShowingNumRaw: CONTESTS_SHOWING_NUM_MIN,
      colorTipDisabled: false,
      contests: [],
      qunyouData: []
    }
  },
  computed: {
    contestsShowingNum() {
      return Math.max(CONTESTS_SHOWING_NUM_MIN, Math.min(this.contests.length, this.contestsShowingNumRaw));
    },
    contestsNumOptions() {
      let options = [CONTESTS_SHOWING_NUM_MIN];
      let div = Math.round(this.contests.length / CONTESTS_SHOWING_NUM_SPAN);
      for (let i=1; i<=div; ++i)
        options.push(Math.round((this.contests.length - CONTESTS_SHOWING_NUM_MIN) / div * i) + CONTESTS_SHOWING_NUM_MIN);
      return options;
    }
  },
  methods: {
  },
  mounted() {
    this.axios.get("./lc-score-board/generateEXCEL/index.xlsx", {
      responseType: "arraybuffer",
      headers: {
        'Cache-Control': 'no-cache',
        'Pragma': 'no-cache',
        'Expires': '0'
      }
    }).then(resp => {
      let data = new Uint8Array(resp.data);
      let wb = XLSX.read(data, {type: 'array', cellStyles: true});
      let ws = wb.Sheets[wb.SheetNames[0]];
      // console.log(ws['F79']);

      // find first row of participants data
      let firstEntryRow = 0;
      while (ws[XLSX.utils.encode_cell({c: 0, r: firstEntryRow})] === undefined)
        ++firstEntryRow;

      // read contests info
      let firstContextCol = 5;
      for (let i = firstContextCol;
           ws[XLSX.utils.encode_cell({r: firstEntryRow-3, c: i})] !== undefined;
           i += 2) {
        this.contests.push({
          id: this.contests.length,
          contestIndex: ws[XLSX.utils.encode_cell({r: firstEntryRow-3, c: i})].v,
          participantNum: ws[XLSX.utils.encode_cell({r: firstEntryRow-2, c: i})].v
        });
      }

      // read each qunyou
      let allDays = [];
      for (let i = firstEntryRow; ws[`A${i+1}`] !== undefined; ++i) {
        let person = {
          cruelRanking: ws[`A${i+1}`].v,
          lcId: ws[`B${i+1}`].v,
          lcLink: ws[`B${i+1}`].l.Rel.Target,
          days: ws[`C${i+1}`].v,
          workNum: 0,
          lcRating: ws[`D${i+1}`].v,
          cruelScore: ws[`E${i+1}`].v,
          contestRankings: [],
          company: ws[XLSX.utils.encode_cell({r: i, c: 5+this.contests.length*2})]?.v.slice(0, -5) ?? '',
          redPacketGroup: ws[`A${i+1}`].s.font.color ? 2 : 1  // 1: A; 2: B
        };
        allDays.push(person.days);
        for (let j=0; j<this.contests.length; ++j) {
          let ranking = ws[XLSX.utils.encode_cell({r: i, c: 5+j*2})].v,
              rankingClr = ws[XLSX.utils.encode_cell({r: i, c: 5+j*2})].s.fill?.fgColor?.rgb.slice(2) ?? 'EAEAEA',
              score = ws[XLSX.utils.encode_cell({r: i, c: 6+j*2})].v;
          person[`contest${this.contests[j].contestIndex}Ranking`] = ranking < 0 ? Infinity : ranking;
          // person[`contest${this.contests[j].contestIndex}RankingClr`] = rankingClr;
          // person[`contest${this.contests[j].contestIndex}Score`] = score;
          person.contestRankings.push({
            ranking: ranking < 0 ? Infinity : ranking,
            rankingClr: rankingClr,
            score: score,
            attendance: ranking
          });
        }
        this.qunyouData.push(person);
        // if (i > 25) break;
      }

      // calculate work number
      allDays.sort((a, b) => b - a);
      let daysToIdx = {};
      for (let i=0; i<allDays.length; ++i)
        if (!(allDays[i] in daysToIdx)) daysToIdx[allDays[i]] = i + 1;
      this.qunyouData.forEach(person => person.workNum = daysToIdx[person.days]);
    });
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
a {
  color: #42b983;
}

.el-checkbox {
  margin-right: 15px;
}

/* element-ui's bug */
#boardTable >>> .el-table__header-wrapper {
  height: 40px;
}
#boardTable >>> .el-table__body-wrapper {
  height: calc(100% - 40px) !important;
}
</style>

<style>
#boardCheckboxGroup .el-checkbox__label {
  padding-left: 6px;
}
</style>