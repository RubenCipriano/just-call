import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { SlideBarComponent } from './slidebar.component';
import { ChannelModule } from '../channel/channel.module';
import { PersonModule } from '../person/person.module';

@NgModule({
  declarations: [SlideBarComponent],
  imports: [CommonModule, ChannelModule, PersonModule],
  exports: [SlideBarComponent],
})
export class SlideBarModule {}