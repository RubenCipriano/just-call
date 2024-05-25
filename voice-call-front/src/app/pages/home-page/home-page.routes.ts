import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HomePageComponent } from './home-page.component';

const routes: Routes = [
  { path: '', component: HomePageComponent },
  { path: ':channelId', loadChildren: () => import('../message-page/message-page.module').then(m => m.MessagePageModule) }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class HomePageRoutingModule {}